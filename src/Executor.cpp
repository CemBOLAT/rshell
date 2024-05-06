#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/File.hpp"
#include "../includes/Directory.hpp"
#include "./Template.cpp"
#include <ctime>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iomanip>
#include <cstring>
#include <stdexcept>

//****************************************  LS  ****************************************//
namespace
{

	// Getting absolute path of a file only by its name while listing files
	string absPathToRelPath(const Shell &shell, const string &path)
	{
		return path.substr(shell.getCurrentDirectory()->getOwnFilesPath().size(), path.size() - 1);
	}
	// lisiting only current directory '.'
	void listOnlyCurrentDirectory(ostream &os, const Directory *dir, size_t maxNameLength)
	{
		time_t rawtime = dir->getTime();
		struct tm *timeinfo = std::localtime(&rawtime);

		os << "D " << std::left << std::setw(maxNameLength) << std::setfill(' ') << "."
		   << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}
	// listing parent directory '..'
	void listParentDirectory(ostream &os, const Directory *dir, size_t maxNameLength)
	{
		time_t rawtime = dir->getParentDirectory()->getTime();
		struct tm *timeinfo = std::localtime(&rawtime);

		os << "D " << std::left << std::setw(maxNameLength) << std::setfill(' ') << ".."
		   << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}
}

namespace
{
	// returns max name length of files in directory to align
	size_t getMaxNameLength(const Directory *directory)
	{
		size_t maxNameLength = 2; // for '.' and '..'
		for (auto file : *directory)
		{
			if (file->getName().length() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		return maxNameLength;
	}
}

namespace Executor
{
	// Precondition: User wants to execute ls command
	// Postcondition: ls command is executed
	// @param directory: current directory
	// @param option: option for ls command
	/*
		ls command is used to list files and directories in current directory.
		ls command's '-R' option has a special print format. Firstly it prints current directory's files and directories.
		Then it prints subdirectories' files and directories recursively.
	*/
	void ls(const Directory *directory, const Shell &Shell, const string &option)
	{
		size_t	maxNameLength = getMaxNameLength(directory); // for align
		listOnlyCurrentDirectory(cout, directory, maxNameLength); // listing '.' is a must for ls command
		if (directory != Shell.getRoot()) // listing '..' is a must except root directory
			listParentDirectory(cout, directory, maxNameLength);
		for (auto file : *directory)
			file->print(cout, maxNameLength);
		if (option == "-R")
		{
			for (auto file : *directory)
			{
				if (dynamic_cast<Directory *>(file))
				{
					Directory *directory = dynamic_cast<Directory *>(file);
					cout << "\n";
					Utils::TextEngine::orange();
					cout << "./" << absPathToRelPath(Shell, directory->getOwnFilesPath()) << ":";
					Utils::TextEngine::reset();
					cout << endl;
					ls(directory, Shell, option);
				}
			}
		}
	}
}

//****************************************  LS OVER ****************************************//

//****************************************  CAT  ****************************************//

namespace Executor
{
	// Precondition: User wants to execute cat command
	// Postcondition: cat command is executed
	void cat(const Shell &shell, const string &fileName)
	{
		File	*filePtr = nullptr;

		if (fileName.empty())
			throw runtime_error("cat: missing operand");
		else if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("cat: " + fileName + ": Is a directory");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName); // get absolute path of file
			if (absPath == "/")
				throw runtime_error("cat: " + fileName + ": Is a directory"); // root directory cannot be printed
			filePtr = File::find<File>(shell, absPath);
			if (filePtr == nullptr)
				throw invalid_argument("cat: " + fileName + ": No such file or directory");
			filePtr->cat(); // <polymorphism>
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

//****************************************  CAT OVER ****************************************//

//****************************************  RM  ****************************************//
namespace Executor
{
	// Precondition: User wants to execute rm command
	// Postcondition: rm command is executed
	void	rm(const Shell &shell, const string &fileName)
	{
		File		*filePtr = nullptr;
		Directory	*parentDirectory = nullptr;

		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			if (absPath == "/" || absPath == shell.getCurrentDirectory()->getOwnFilesPath()) // if file is root directory or current directory throw exception
				throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory"); // root directory cannot be deleted
			filePtr = File::find<File>(shell, absPath);
			parentDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absPath));
			if (filePtr == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			if (dynamic_cast<Directory *>(filePtr))
				throw invalid_argument("rm: cannot remove '" + fileName + "': Is a directory"); // directory cannot be deleted as you said in pdf
			parentDirectory->removeFile(filePtr->getName()); // remove file from parent directory
			parentDirectory->setTime(time(nullptr)); // set parent directory's time to current time to show that it is modified
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

//****************************************  RM OVER ****************************************//
//****************************************  MKDIR  ****************************************//

namespace Executor
{
	// Precondition: User wants to execute mkdir command
	// Postcondition: mkdir command is executed
	// @brief : '' Makes or removes a directory '' as pdf says
	void mkdir(const Shell &shell, const string &fileName)
	{
		File		*directory = nullptr;
		Directory	*parentDirectory = nullptr;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			if (absPath == "/" || absPath == shell.getCurrentDirectory()->getOwnFilesPath()) // if file is root directory or current directory throw exception
				throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists"); // root directory cannot be created and deleted
			string pPath = Utils::getParentPathOfAbsPath(absPath);
			if (pPath == "/")
				pPath = ""; // if parent directory is root directory, parent directory's path is empty
			directory = File::find<File>(shell, absPath);
			parentDirectory = File::find<Directory>(shell, pPath);
			if (directory != nullptr) // delete if existent file is directory and throw exception if not
			{
				if (dynamic_cast<Directory *>(directory)) // if file is directory
				{
					parentDirectory->removeFile(absPath.substr(absPath.find_last_of('/') + 1)); // remove directory from parent directory
					parentDirectory->setTime(time(nullptr));
				}
				else
					throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists"); // if file is not directory throw exception
				return;
			}
			else if (parentDirectory == nullptr) // if parent directory does not exist throw exception <for root directory parent directory manipulalated as root too at find function>
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': No such file or directory");
			string name = absPath.substr(absPath.find_last_of('/') + 1);
			if (parentDirectory == shell.getRoot()) // if parent directory is root directory
				directory = new Directory(name,
										  time(nullptr), parentDirectory->getPath(), parentDirectory);
			else
				directory = new Directory(name,
										  time(nullptr), parentDirectory->getPath() + parentDirectory->getName() + "/", parentDirectory);
			parentDirectory->addFile(directory); // add directory to parent directory
			parentDirectory->setTime(time(nullptr)); // set parent directory's time to current time to show that it is modified
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}
//****************************************  MKDIR OVER ****************************************//
//****************************************  CD  ****************************************//
namespace Executor
{
	// Precondition: User wants to execute cd command
	// Postcondition: cd command is executed
	// @brief : '' Changes the current directory '' as pdf says
	// @brief '.' and '..' are special cases for cd command and empty string is special case for cd command
	// @brief if directory is not existent throw exception
	void	cd(Shell &shell, const string &directoryName)
	{
		File	*directory = nullptr;
		if (directoryName.empty()) // if directory name is empty set current directory to root directory
			shell.setCurrentDirectory(shell.getRoot());
		else if (directoryName == ".") // if directory name is '.' do nothing
			return;
		else if (directoryName == "..") // if directory name is '..' set current directory to parent directory
		{
			if (shell.getCurrentDirectory() == shell.getRoot()) // if current directory is root directory do nothing
				return;
			shell.setCurrentDirectory(shell.getCurrentDirectory()->getParentDirectory());
		}
		else
		{
			try
			{
				string pPath = Utils::relPathToAbsPath(shell, directoryName);
				if (pPath == "/")
				{
					shell.setCurrentDirectory(shell.getRoot());
					return;
				}
				directory = File::find<File>(shell, pPath);
				if (directory == nullptr)
					throw invalid_argument("cd: " + directoryName + ": No such file or directory");
				directory->cd(shell); // <polymorphism>
			}
			catch (const invalid_argument &e)
			{
				throw e;
			}
		}
	}
}

// ****************************************  CD OVER ****************************************//
// ****************************************  CP  ****************************************//

namespace
{
	// Precondition: User wants to execute cp command and source is a regular file
	// Postcondition: cp command is executed and source is copied to destination
	// @brief : '' Copies file and directory from our original os '' as pdf says
	RegularFile	*copyRegularFile(const string &source, const string &fileName, const struct stat &sourceStat, const string &path)
	{
		ifstream	sourceFile(source);
		RegularFile	*regularFile;
		string		data, line;
		if (!sourceFile.is_open())
			throw runtime_error("cp: source file '" + source + "' does not exist");
		while (getline(sourceFile, line)) // read file line by line
		{
			data += line + "\n";
		}
		data = data.substr(0, data.size() - 2);
		data += static_cast<char>(3); // end of text character
		sourceFile.close();
		regularFile = new RegularFile(fileName, data.size(), sourceStat.st_mtime, data, path); // create regular file
		return regularFile;
	}
}

namespace
{
	// Precondition: User wants to execute cp command and source is a directory
	// Postcondition: cp command is executed and source is copied to destination
	Directory *copyDirectory(const string &source, const string &fileName, const Shell &shell, const struct stat &sourceStat, const string &path, Directory *parentDirectory)
	{
		Directory	*directory; // directory to be copied

		DIR *copiedDir = opendir(source.c_str()); // open directory
		if (!copiedDir)
		{
			throw std::runtime_error("cp: cannot open source directory '" + source + "'");
		}
		struct dirent *entry; // entry of directory
		directory = new Directory(fileName, sourceStat.st_mtime, path, parentDirectory); // create directory
		while ((entry = readdir(copiedDir)) != nullptr) // read the directory until end
		{
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) // ignore '.' and '..'
			{
				std::string entryPath = source + "/" + entry->d_name; // get entry's path

				struct stat entryStat; // entry's stat
				if (stat(entryPath.c_str(), &entryStat) != 0)
				{
					throw std::runtime_error("cp: error accessing file '" + entryPath + "'"); // if stat cannot be accessed throw exception and exit
				}
				if (S_ISREG(entryStat.st_mode))
				{
					// Copy regular file
					RegularFile *regularFile = copyRegularFile(entryPath, entry->d_name, entryStat, directory->getOwnFilesPath());
					directory->addFile(regularFile); // add regular file to directory
				}
				else if (S_ISDIR(entryStat.st_mode))
				{
					// Recursively copy subdirectories
					Directory *subDirectory = copyDirectory(entryPath, entry->d_name, shell, entryStat, directory->getOwnFilesPath(), directory);
					directory->addFile(subDirectory); // add subdirectory to directory
				}
			}
		}
		closedir(copiedDir); // close directory
		return (directory);
	}
}

namespace
{
	// Precondition: User wants to execute cp command so program gets the size of directory
	// Postcondition: Returns the size of directory
	size_t getDirectorySize(const struct stat &sourcestat, const string &source)
	{
		DIR *copiedDir = opendir(source.c_str());
		if (!copiedDir)
		{
			throw std::runtime_error("cp: cannot open source directory '" + source + "'");
		}
		(void)sourcestat; // unused
		struct dirent *entry;
		size_t size = 0;
		while ((entry = readdir(copiedDir)) != nullptr)
		{
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				std::string entryPath = source + "/" + entry->d_name;

				struct stat entryStat;
				if (stat(entryPath.c_str(), &entryStat) != 0)
				{
					throw std::runtime_error("cp: error accessing file '" + entryPath + "'");
				}

				if (S_ISREG(entryStat.st_mode))
				{
					size += entryStat.st_size;
				}
				else if (S_ISDIR(entryStat.st_mode))
				{
					size += getDirectorySize(entryStat, entryPath); // recursively get subdirectories' size
				}
			}
		}
		closedir(copiedDir);
		return (size);
	}
}

namespace
{
	// Precondition: User wants to execute cp command
	// Postcondition: cp command is executed and source is copied to destination
	void onlyAddToDirectory(const Shell &shell, const string &source, const string &fileName, const struct stat &sourceStat)
	{

		if (S_ISREG(sourceStat.st_mode))
		{
			RegularFile *regularFile = copyRegularFile(source, fileName, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(regularFile);
			shell.getCurrentDirectory()->setTime(time(nullptr));
		}
		else if (S_ISDIR(sourceStat.st_mode))
		{
			Directory *directory = copyDirectory(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath(), shell.getCurrentDirectory());
			shell.getCurrentDirectory()->addFile(directory);
		}
		shell.getCurrentDirectory()->setTime(time(nullptr));
	}
}

namespace Executor
{
	// Precondition: User wants to execute cp command
	// Postcondition: program executes cp command
	void cp(const Shell &shell, const string &source, const string &fileName)
	{

		struct stat	sourceStat;
		File		*file = nullptr;

		if (source.empty() || fileName.empty())
			throw runtime_error("cp: missing operand");
		if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("cp: cannot copy to '" + fileName + "': File exists");
		if (stat(source.c_str(), &sourceStat) != 0)
		{
			throw std::runtime_error("cp: source file '" + source + "' does not exist"); // if stat cannot be accessed throw exception and exit
		}
		file = File::find<File>(shell, shell.getCurrentDirectory()->getOwnFilesPath() + fileName);
		if (file != nullptr)
			throw runtime_error("cp: cannot copy to '" + fileName + "': File exists"); // if file exists throw exception
		if (S_ISREG(sourceStat.st_mode) && sourceStat.st_size + Utils::getProgramSize(shell) > shell.getOsSize()) // size of file + size of program > size of os
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		if (S_ISDIR(sourceStat.st_mode) && getDirectorySize(sourceStat, source) + Utils::getProgramSize(shell) > shell.getOsSize())
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		onlyAddToDirectory(shell, source, fileName, sourceStat); // add file to current directory
	}
}

// ****************************************  CP OVER ****************************************//
// ****************************************  LINK  ****************************************//
namespace Executor
{
	// Precondition: User wants to execute link command
	// Postcondition: program executes link command
	void link(const Shell &shell, const string &source, const string &dest)
	{
		Directory		*destDirectory = nullptr;
		Directory		*sourceDirectory = nullptr;
		File			*sourceFile = nullptr;
		File			*destFile = nullptr;
		SymbolicLink	*symbolicLink = nullptr; // symbolic link to be created

		string absSourcePath = Utils::relPathToAbsPath(shell, source);
		string absDestPath = Utils::relPathToAbsPath(shell, dest);
		if (dest.empty() || source.empty())
			throw runtime_error("link: missing operand");
		else if (dest == "." || dest == ".." || dest == "/")
			throw runtime_error("link: cannot create link '" + dest + "': File exists");
		try
		{
			destFile = File::find<File>(shell, absDestPath);
			destDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absDestPath));
			if (destDirectory == nullptr) // if destination directory does not exist throw exception
				throw invalid_argument("link: cannot create link '" + dest + "': No such file or directory");
			if (destFile != nullptr) // if destination file exists throw exception
				throw invalid_argument("link: cannot create link '" + dest + "': File exists");
			sourceFile = File::find<File>(shell, absSourcePath);
			sourceDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absSourcePath));
			if (sourceDirectory == nullptr){
				symbolicLink = new SymbolicLink(absDestPath.substr(absDestPath.find_last_of('/') + 1), destDirectory->getOwnFilesPath(), time(nullptr), sourceFile,
											absSourcePath.substr(absSourcePath.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absSourcePath) + "/");
			}
			else {
				symbolicLink = new SymbolicLink(absDestPath.substr(absDestPath.find_last_of('/') + 1), destDirectory->getOwnFilesPath(), time(nullptr), sourceFile,
											absSourcePath.substr(absSourcePath.find_last_of('/') + 1), sourceDirectory->getOwnFilesPath());
			}
			destDirectory->addFile(symbolicLink); // add symbolic link to destination directory
			destDirectory->setTime(time(nullptr)); // set destination directory's time to current time to show that it is modified
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

// ****************************************  LINK OVER ****************************************//

namespace Executor
{
	// Precondition: User wants to execute clear command
	// Postcondition: program executes clear command
	void clear()
	{
		cout << "\033[2J\033[1;1H"; // clear screen
	}
}


namespace Executor
{
	// Precondition: User wants to execute touch command
	// Postcondition: program executes touch command
	void touch(const Shell &shell, const vector<string> &splitpath)
	{
		Directory	*parentDirectory = nullptr;
		File		*file = nullptr;
		string		fileName;
		string		absPath;
		if (splitpath.size() < 2)
			throw runtime_error("touch: missing operand");
		else if (splitpath.size() > 2)
			throw runtime_error("touch: invalid number of arguments for cemalBolatShell -- ");
		try
		{
			absPath = Utils::relPathToAbsPath(shell, splitpath[1]);
			if (absPath == "/" || absPath == shell.getCurrentDirectory()->getOwnFilesPath()) // if file is root directory or current directory throw exception
				throw runtime_error("touch: cannot create file '" + splitpath[1] + "': File exists"); // root directory cannot be created and deleted
			file = File::find<File>(shell, absPath);
			parentDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absPath));
			if (file != nullptr) // delete if existent file is directory and throw exception if not
			{
				if (dynamic_cast<Directory *>(file)) // if file is directory
				{
					parentDirectory->removeFile(absPath.substr(absPath.find_last_of('/') + 1)); // remove directory from parent directory
					parentDirectory->setTime(time(nullptr));
				}
				else
					file->setTime(time(nullptr)); // if file is not directory throw exception
				return;
			}
			else if (parentDirectory == nullptr) // if parent directory does not exist throw exception <for root directory parent directory manipulalated as root too at find function>
				throw invalid_argument("touch: cannot create file '" + splitpath[1] + "': No such file or directory");
			fileName = absPath.substr(absPath.find_last_of('/') + 1);
			file = new RegularFile(fileName, 0, time(nullptr), "", parentDirectory->getOwnFilesPath());
			parentDirectory->addFile(file); // add directory to parent directory
			parentDirectory->setTime(time(nullptr)); // set parent directory's time to current time to show that it is modified
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

namespace Executor
{
	// Precondition: User wants to execute echo command
	// Postcondition: program executes echo command
	void echo(const Shell &shell, const string &text, const string &fileName)
	{
		Directory	*parentDirectory = nullptr;
		File		*file = nullptr;
		string		absPath;
		if (fileName.empty())
			throw runtime_error("echo: missing operand");
		try
		{
			absPath = Utils::relPathToAbsPath(shell, fileName);
			if (absPath == "/" || absPath == shell.getCurrentDirectory()->getOwnFilesPath()) // if file is root directory or current directory throw exception
				throw runtime_error("echo: cannot create file '" + fileName + "': File exists"); // root directory cannot be created and deleted
			file = File::find<File>(shell, absPath);
			parentDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absPath));
			if (file != nullptr) // delete if existent file is directory and throw exception if not
			{
				if (dynamic_cast<Directory *>(file)) // if file is directory
				{
					parentDirectory->removeFile(absPath.substr(absPath.find_last_of('/') + 1)); // remove directory from parent directory
					parentDirectory->setTime(time(nullptr));
				}
				else
					file->setTime(time(nullptr)); // if file is not directory throw exception
				return;
			}
			else if (parentDirectory == nullptr) // if parent directory does not exist throw exception <for root directory parent directory manipulalated as root too at find function>
				throw invalid_argument("echo: cannot create file '" + fileName + "': No such file or directory");
			file = new RegularFile(absPath.substr(absPath.find_last_of('/') + 1), text.size(), time(nullptr), text, parentDirectory->getOwnFilesPath());
			parentDirectory->addFile(file); // add directory to parent directory
			parentDirectory->setTime(time(nullptr)); // set parent directory's time to current time to show that it is modified
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}