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

namespace
{
	void listOnlyCurrentDirectory(ostream &os, const Shell &shell, size_t maxNameLength)
	{
		time_t rawtime = shell.getRoot()->getTime();
		struct tm *timeinfo = std::localtime(&rawtime);

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << "."
		   << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}

	void listSpecialDirectories(ostream &os, const Shell &shell, size_t maxNameLength)
	{
		time_t rawtime = shell.getCurrentDirectory()->getTime();
		struct tm *timeinfo = std::localtime(&rawtime);

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << " ."
		   << " ";
		Utils::printTime(os, timeinfo) << std::endl;

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << ".."
		   << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}
}

namespace Executor
{
	void ls(const Shell &Shell)
	{
		Directory *currentDirectory = Shell.getCurrentDirectory();
		vector<File *> files = currentDirectory->getFiles();
		size_t maxNameLength = 0;

		for (File *file : files)
		{
			if (file->getName().size() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		if (Shell.getCurrentDirectory() != Shell.getRoot())
		{
			listSpecialDirectories(cout, Shell, maxNameLength);
		}
		else
		{
			listOnlyCurrentDirectory(cout, Shell, maxNameLength);
		}
		if (files.empty())
			return;

		for (File *file : files)
		{
			file->print(cout, maxNameLength);
		}
	}
}

namespace Executor
{
	void cat(const Shell &shell, const string &fileName)
	{
		File *filePtr = nullptr;

		if (fileName.empty())
			throw runtime_error("cat: missing operand");
		else if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("cat: " + fileName + ": Is a directory");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			filePtr = File::find<File>(shell, absPath);
			if (filePtr == nullptr)
				throw invalid_argument("cat: " + fileName + ": No such file or directory");
			filePtr->cat();
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

namespace Executor
{
	void rm(const Shell &shell, const string &fileName)
	{
		File *filePtr = nullptr;
		Directory *parentDirectory = nullptr;

		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == ".." || fileName == "/")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			filePtr = File::find<File>(shell, absPath);
			if (absPath[absPath.size() - 1] == '/')
			{
				absPath = absPath.substr(0, absPath.size() - 1);
			}
			parentDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absPath));
			if (filePtr == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			if (dynamic_cast<Directory *>(filePtr))
				throw invalid_argument("rm: cannot remove '" + fileName + "': Is a directory");
			parentDirectory->removeFile<File>(filePtr->getName());
			parentDirectory->setTime(time(nullptr));
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

namespace Executor
{
	void mkdir(const Shell &shell, const string &fileName)
	{
		File *directory = nullptr;
		Directory *parentDirectory = nullptr;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		try
		{
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			if (absPath[absPath.size() - 1] == '/')
			{
				absPath = absPath.substr(0, absPath.size() - 1);
			}

			string pPath = Utils::getParentPathOfAbsPath(absPath);
			if (pPath[pPath.size() - 1] == '/')
			{
				pPath = pPath.substr(0, pPath.size() - 1);
			}

			directory = File::find<File>(shell, absPath);
			parentDirectory = File::find<Directory>(shell, pPath);
			if (directory != nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists");
			else if (parentDirectory == nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': No such file or directory");
			string name = absPath.substr(absPath.find_last_of('/') + 1);
			if (parentDirectory == shell.getRoot())
				directory = new Directory(name,
										  time(nullptr), parentDirectory->getPath(), parentDirectory);
			else
				directory = new Directory(name,
										  time(nullptr), parentDirectory->getPath() + parentDirectory->getName() + "/", parentDirectory);
			parentDirectory->addFile(directory);
			parentDirectory->setTime(time(nullptr));
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

namespace
{
	void changeDir(Shell &shell, const string &directoryName)
	{
		Directory *directory = nullptr;
		if (directoryName.empty())
			throw runtime_error("cd: missing operand");
		else if (directoryName == "." || directoryName == "..")
			throw runtime_error("cd: " + directoryName + ": Not a directory");
		try
		{
			string pPath = Utils::relPathToAbsPath(shell, directoryName);
			directory = File::find<Directory>(shell, pPath);
			if (directory == nullptr)
				throw invalid_argument("cd: " + directoryName + ": No such file or directory");
			shell.setCurrentDirectory(directory);
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}

namespace Executor
{
	// path bir kısmı düzgün sadece ./a/../b/ vs yok
	void cd(Shell &shell, const string &directoryName)
	{
		if (directoryName.empty())
		{
			shell.setCurrentDirectory(shell.getRoot());
		}
		else if (directoryName == ".")
			return;
		else if (directoryName == "..")
		{
			if (shell.getCurrentDirectory() == shell.getRoot())
				return;
			shell.setCurrentDirectory(shell.getCurrentDirectory()->getParentDirectory());
		}
		else
		{
			try
			{
				changeDir(shell, directoryName);
			}
			catch (const runtime_error &e)
			{
				throw e;
			}
			catch (const invalid_argument &e)
			{
				throw e;
			}
		}
	}
}

namespace Executor
{
	void lsRecursive(const Directory *directory, const Shell &Shell)
	{
		vector<File *> files = directory->getFiles();
		size_t maxNameLength = 0;
		for (File *file : files)
		{
			if (file->getName().length() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		if (maxNameLength < 2)
			maxNameLength = 2;
		if (directory->getPath() + directory->getName() != "//")
		{
			listSpecialDirectories(cout, Shell, maxNameLength);
		}
		else
		{
			listOnlyCurrentDirectory(cout, Shell, maxNameLength);
		}
		for (File *file : files)
		{
			file->print(cout, maxNameLength);
		}
		for (File *file : files)
		{
			if (dynamic_cast<Directory *>(file))
			{
				Directory *directory = dynamic_cast<Directory *>(file);
				cout << "\n";
				Utils::TextEngine::greenBackground();
				cout << "." << directory->getPath() + directory->getName() << ":";
				Utils::TextEngine::reset();
				cout << endl;
				lsRecursive(directory, Shell);
			}
		}
	}
}

namespace
{
	RegularFile *copyRegularFile(const string &source, const string &fileName, const Shell &shell, const struct stat &sourceStat, const string &path)
	{
		ifstream sourceFile(source);
		RegularFile *regularFile;
		string data, line;
		(void)shell;
		if (!sourceFile.is_open())
			throw runtime_error("file cannot open at your OS");
		while (getline(sourceFile, line))
		{
			data += line + "\n";
		}
		data = data.substr(0, data.size() - 2);
		data += static_cast<char>(3);
		sourceFile.close();
		regularFile = new RegularFile(fileName, data.size(), sourceStat.st_mtime, data, path);
		return regularFile;
	}
}

namespace
{
	Directory *copyDirectory(const string &source, const string &fileName, const Shell &shell, const struct stat &sourceStat, const string &path)
	{
		Directory *directory;

		DIR *copiedDir = opendir(source.c_str());
		if (!copiedDir)
		{
			throw std::runtime_error("cp: cannot open source directory '" + source + "'");
		}
		struct dirent *entry;
		directory = new Directory(fileName, sourceStat.st_mtime, path, shell.getCurrentDirectory());
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
					// Copy regular file
					RegularFile *regularFile = copyRegularFile(entryPath, entry->d_name, shell, entryStat, directory->getOwnFilesPath());
					directory->addFile(regularFile);
				}
				else if (S_ISDIR(entryStat.st_mode))
				{
					// Recursively copy subdirectories
					Directory *subDirectory = copyDirectory(entryPath, entry->d_name, shell, entryStat, directory->getOwnFilesPath());
					directory->addFile(subDirectory);
				}
			}
		}
		closedir(copiedDir);
		return (directory);
	}
}

namespace
{
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
					size += getDirectorySize(entryStat, entryPath);
				}
				else
				{
					size += 2;
				}
			}
		}
		closedir(copiedDir);
		return (size);
	}
}

namespace
{
	void onlyAddToDirectory(const Shell &shell, const string &source, const string &fileName, const struct stat &sourceStat)
	{

		if (S_ISREG(sourceStat.st_mode))
		{
			RegularFile *regularFile = copyRegularFile(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(regularFile);
		}
		else if (S_ISDIR(sourceStat.st_mode))
		{
			Directory *directory = copyDirectory(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(directory);
		}
		shell.getCurrentDirectory()->setTime(time(nullptr));
	}
}

// linklere bak
namespace Executor
{
	void cp(const Shell &shell, const string &source, const string &fileName)
	{

		struct stat sourceStat;
		File *file = nullptr;

		if (source.empty() || fileName.empty())
			throw runtime_error("cp: missing operand");
		if (stat(source.c_str(), &sourceStat) != 0)
		{
			throw std::runtime_error("cp: source file '" + source + "' does not exist");
		}
		std::cout << "path : " << shell.getCurrentDirectory()->getOwnFilesPath() + fileName << std::endl;
		file = File::find<File>(shell, shell.getCurrentDirectory()->getOwnFilesPath() + fileName);
		if (S_ISREG(sourceStat.st_mode) && sourceStat.st_size + Utils::getProgramSize(shell.getRoot()) > shell.getOsSize())
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		if (S_ISDIR(sourceStat.st_mode) && getDirectorySize(sourceStat, source) + Utils::getProgramSize(shell.getRoot()) > shell.getOsSize())
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		if (file == nullptr)
			onlyAddToDirectory(shell, source, fileName, sourceStat);
		else if ((dynamic_cast<Directory *>(file) && !S_ISDIR(sourceStat.st_mode)) ||
				 (dynamic_cast<RegularFile *>(file) && !S_ISREG(sourceStat.st_mode)))
		{
			throw runtime_error("cp: cannot copy '" + source + "' -- '" + fileName + "' : File exists");
		}
		else
		{
			shell.getCurrentDirectory()->removeFile<File>(fileName);
			onlyAddToDirectory(shell, source, fileName, sourceStat);
		}
		shell.getCurrentDirectory()->setTime(time(nullptr));
	}
}

// copy isLNK SAÇMALIK VAR
// @brief optimazsion probs in here
namespace Executor
{
	void	link(const Shell &shell, const string &source, const string &dest)
	{
		Directory		*destDirectory = nullptr;
		Directory		*sourceDirectory = nullptr;
		File			*sourceFile = nullptr;
		File			*destFile = nullptr;
		SymbolicLink	*symbolicLink = nullptr;

		string		absSourcePath = Utils::relPathToAbsPath(shell, source);
		string		absDestPath = Utils::relPathToAbsPath(shell, dest);
		if (dest.empty() || source.empty())
			throw runtime_error("link: missing operand");
		else if (dest == "." || dest == "..")
			throw runtime_error("link: cannot create link '" + dest + "': File exists");
		try
		{
			destFile = File::find<File>(shell, absDestPath);
			if (destFile != nullptr)
				throw invalid_argument("link: cannot create link '" + dest + "': File exists");
			sourceFile = File::find<File>(shell, absSourcePath);
			destDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absDestPath));
			sourceDirectory = File::find<Directory>(shell, Utils::getParentPathOfAbsPath(absSourcePath));
			std::cout << "source file null" << std::endl;
			symbolicLink = new SymbolicLink(absDestPath.substr(absDestPath.find_last_of('/') + 1)
										,destDirectory->getOwnFilesPath(), time(nullptr),sourceFile,
										absSourcePath.substr(absSourcePath.find_last_of('/') + 1), sourceDirectory->getOwnFilesPath());
			// root trick sonra bak!
			destDirectory->addFile(symbolicLink);
			destDirectory->setTime(time(nullptr));
			sourceFile = File::find<RegularFile>(shell, absSourcePath); // super saçma bir hata var burda
		}
		catch (const invalid_argument &e)
		{
			throw e;
		}
	}
}
