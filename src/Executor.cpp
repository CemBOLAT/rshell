#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Directory.hpp"
#include "./Template.cpp"
#include <ctime>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

namespace {
	void listOnlyCurrentDirectory(ostream &os, const Shell& shell, size_t maxNameLength) {
		time_t rawtime = shell.getRoot()->getTime();
		struct tm* timeinfo = std::localtime(&rawtime);

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << "." << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}

	void listSpecialDirectories(ostream &os, const Shell& shell, size_t maxNameLength) {
		time_t rawtime = shell.getCurrentDirectory()->getTime();
		struct tm* timeinfo = std::localtime(&rawtime);

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << " ." << " ";
			Utils::printTime(os, timeinfo) << std::endl;

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << ".." << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}
}

namespace Executor {

	void ls(const Shell& Shell) {
		Directory* currentDirectory = Shell.getCurrentDirectory();
		vector<File *> files = currentDirectory->getFiles();
		size_t maxNameLength = 0;
		for (File *file : files)
		{
			if (file->getName().size() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		if (Shell.getCurrentDirectory() != Shell.getRoot()) {
			listSpecialDirectories(cout, Shell, maxNameLength);
		}
		else {
			listOnlyCurrentDirectory(cout, Shell, maxNameLength);
		}
		if (files.empty())
			return;

		for (File* file : files) {
			file->print(cout, maxNameLength);
			//else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			//}
		}
	}
}

namespace Executor {
	void cat(const Shell& shell, const string& fileName) {
		RegularFile		*regularFile = nullptr;
		SymbolicLink	*symbolicLink = nullptr;
		Directory		*directory = nullptr;

		if (fileName.empty())
			throw runtime_error("cat: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("cat: " + fileName + ": Is a directory");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			regularFile = RegularFile::find(shell, absPath, nullptr);
			symbolicLink = SymbolicLink::find(shell, absPath, nullptr);
			directory = Directory::find(shell, absPath, nullptr);
			if (regularFile == nullptr && symbolicLink == nullptr && directory == nullptr)
				throw invalid_argument("cat: " + fileName + ": No such file or directory");
			else if (regularFile != nullptr)
				regularFile->cat();
			else if (directory != nullptr)
				directory->cat();
			else if (symbolicLink != nullptr)
				symbolicLink->cat();
		} catch (const runtime_error& e) {
			throw e;
		} catch (const invalid_argument& e) {
			throw e;
		}
		// cout << regularFile->getData() << endl;
	}
}

namespace Executor {
	void rm(const Shell& shell, const string& fileName) {
		RegularFile		*regularFile = nullptr;
		Directory		*directory = nullptr;
		SymbolicLink	*symbolicLink = nullptr;
		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			regularFile = RegularFile::find(shell, absPath, nullptr);
			symbolicLink = SymbolicLink::find(shell, absPath, nullptr);
			//regularFile = Utils::findRegularFile(shell, absPath);
			directory = Directory::find(shell, absPath.substr(0, absPath.find_last_of('/')), nullptr);

			//directory = Utils::findDirectory(shell, absPath.substr(0, absPath.find_last_of('/')));
			//std::cout << fileName << " AA" << std::endl;

			if (regularFile == nullptr && symbolicLink == nullptr && directory == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			else if (regularFile != nullptr)
				directory->removeFile<RegularFile>(regularFile->getName());
			else if (symbolicLink != nullptr)
				directory->removeFile<SymbolicLink>(symbolicLink->getName());
			else if (directory != nullptr)
				throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory");
		} catch (const runtime_error& e) {
			throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory");
		} catch (const invalid_argument& e) {
			throw e;
		}
	}
}

namespace Executor{
	void mkdir(const Shell& shell, const string& fileName) {
		Directory *directory = nullptr;
		Directory *parentDirectory = nullptr;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			directory = Directory::find(shell, absPath, nullptr);
			//directory = Utils::findDirectory(shell, absPath);
			//std::cout << "abs BB : " << Utils::relPathToAbsPath(shell, fileName) << std::endl;
			string pPath = Utils::getParentPathOfAbsPath(Utils::relPathToAbsPath(shell, fileName));
			//std::cout << "abs AA : " << pPath << std::endl;
			parentDirectory = Directory::find(shell, pPath, nullptr);
			//parentDirectory = Utils::findDirectory(shell, pPath);
			if (directory != nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists");
			else if (parentDirectory == nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': No such file or directory");
			string name = fileName.substr(fileName.find_last_of('/') + 1);
			if (parentDirectory == shell.getRoot())
				directory = new Directory(name,
										time(nullptr), parentDirectory->getPath(), parentDirectory);
			else
				directory = new Directory(name,
										time(nullptr), parentDirectory->getPath() + parentDirectory->getName() + "/", parentDirectory);
			parentDirectory->addFile(directory);
		} catch (const runtime_error& e) {
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		} catch (const invalid_argument& e) {
			throw e;
		}

	}
}

namespace {
	void	changeDir(Shell& shell, const string& directoryName) {
		Directory *directory = nullptr;
		if (directoryName.empty())
			throw runtime_error("cd: missing operand");
		else if (directoryName == "." || directoryName == "..")
			throw runtime_error("cd: " + directoryName + ": Not a directory");
		try {
			string pPath = Utils::relPathToAbsPath(shell, directoryName);
			directory = Directory::find(shell, pPath, nullptr);
			//directory = Utils::findDirectory(shell, pPath);
			if (directory == nullptr)
				throw invalid_argument("cd: " + directoryName + ": No such file or directory");
			shell.setCurrentDirectory(directory);
		} catch (const invalid_argument& e) {
			throw e;
		}
	}
}

namespace Executor {
	// path bir kısmı düzgün sadece ./a/../b/ vs yok
	void cd(Shell& shell, const string& directoryName) {
		if (directoryName.empty()){
			shell.setCurrentDirectory(shell.getRoot());
		}
		else if (directoryName == ".")
			return;
		else if (directoryName == ".."){
			if (shell.getCurrentDirectory() == shell.getRoot())
				return;
			shell.setCurrentDirectory(shell.getCurrentDirectory()->getParentDirectory());
		}
		else {
			try {
				changeDir(shell, directoryName);
			} catch (const runtime_error& e) {
				throw e;
			} catch (const invalid_argument& e) {
				throw e;
			}
		}

	}
}



namespace Executor {
	void lsRecursive(const Directory *directory, const Shell& Shell) {
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
		for (auto file : files)
		{
			if (file->getName().size() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		for (File *file : files)
		{
			file->print(cout, maxNameLength);
			// else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			// }
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
				// (. ve ..) var untuma
				lsRecursive(directory, Shell); // sonra karar verirsin
			}
		}
	}
}

namespace {
	RegularFile *copyRegularFile(const string &source, const string &fileName, const Shell& shell, const struct stat &sourceStat, const string &path){
		ifstream		sourceFile(source);
		RegularFile		*regularFile;
		string			data, line;
		if (!sourceFile.is_open())
			throw runtime_error("file cannot open at your OS");
		while (getline(sourceFile, line))
		{
			data += line + "\n";
		}
		data = data.substr(0, data.size() - 1);
		data += static_cast<char>(3);
		sourceFile.close();
		regularFile = new RegularFile(fileName, data.size(), sourceStat.st_mtime, data, path);
		return regularFile;
	}
}

namespace {
	Directory *copyDirectory(const string &source, const string &fileName, const Shell& shell, const struct stat &sourceStat, const string &path){
		Directory		*directory;

		DIR *copiedDir = opendir(source.c_str());
		if (!copiedDir)
		{
			throw std::runtime_error("cp: cannot open source directory '" + source + "'");
		}
		struct dirent	*entry;
		directory = new Directory(fileName, sourceStat.st_mtime, path, shell.getCurrentDirectory()->getParentDirectory());
		while ((entry = readdir(copiedDir)) != nullptr)
		{
			if (std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0)
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
				else
				{
					// Handle other types of files if needed
				}
			}
		}
		closedir(copiedDir);
		return (directory);
	}
}

namespace {
	size_t	getDirectorySize(const struct stat &sourcestat, const string &source){
		DIR *copiedDir = opendir(source.c_str());
		if (!copiedDir)
		{
			throw std::runtime_error("cp: cannot open source directory '" + source + "'");
		}
		struct dirent	*entry;
		size_t size = 0;
		while ((entry = readdir(copiedDir)) != nullptr)
		{
			if (std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0)
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
					// Handle other types of files if needed
				}
			}
		}
		closedir(copiedDir);
		return (size);
	}
}

// linklere bak
namespace Executor {
	void cp(const Shell& shell, const string &source, const string &fileName){

		struct stat	sourceStat;

		if (source.empty() || fileName.empty())
			throw runtime_error("cp: missing operand");
		if (stat(source.c_str(), &sourceStat) != 0)
		{
			throw std::runtime_error("cp: source file '" + source + "' does not exist");
		}
		RegularFile *regularFile;

		regularFile = RegularFile::find(shell, fileName, nullptr);
		Directory *directory;
		directory = Directory::find(shell, fileName, nullptr);
		// size is not proper for directory

		if (S_ISREG(sourceStat.st_mode)  && sourceStat.st_size + Utils::getProgramSize(shell.getRoot()) > shell.getOsSize())
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		else if (S_ISDIR(sourceStat.st_mode) && getDirectorySize(sourceStat, source) + Utils::getProgramSize(shell.getRoot()) > shell.getOsSize())
			throw runtime_error("cp: cannot copy '" + source + "': No space left on device");
		if (regularFile == nullptr && S_ISREG(sourceStat.st_mode)){
			//std::cout << "buraya girdi" << std::endl;
			regularFile = copyRegularFile(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(regularFile);
		}
		else if (directory == nullptr && S_ISDIR(sourceStat.st_mode)){ // reküfsif
			directory = copyDirectory(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(directory);
		}
		else if (directory && S_ISDIR(sourceStat.st_mode)){ // rekürsif üstüne yaz
			shell.getCurrentDirectory()->removeFile<Directory>(fileName);
			directory = copyDirectory(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(directory);
		}
		else if (regularFile && S_ISREG(sourceStat.st_mode)){
			//std::cout << "buraya gir11di" << std::endl;
			shell.getCurrentDirectory()->removeFile<RegularFile>(fileName);
			regularFile = copyRegularFile(source, fileName, shell, sourceStat, shell.getCurrentDirectory()->getOwnFilesPath());
			shell.getCurrentDirectory()->addFile(regularFile);
		}
	}

}


// @brief optimazsion probs in here
namespace Executor {
	void	link(const Shell& shell, const string &source, const string &dest){
		Directory		*sourceDirectory = nullptr;
		Directory		*destDirectory = nullptr;
		File			*sourceFile = nullptr;
		File *destFile = nullptr;
		string absSourcePath = Utils::relPathToAbsPath(shell, source);
		string absDestPath = Utils::relPathToAbsPath(shell, dest);
		if (dest.empty() || source.empty())
			throw runtime_error("link: missing operand");
		else if (dest == "." || dest == "..")
			throw runtime_error("link: cannot create link '" + dest + "': File exists");
		try {
			destFile = SymbolicLink::find(shell, absDestPath, nullptr);
			if (destFile != nullptr) // dest symbolic mi
				return;
			destFile = RegularFile::find(shell, absDestPath, nullptr);
			if (destFile != nullptr) // dest regular mı
				return;
			destDirectory = Directory::find(shell, absDestPath, nullptr);
			if (destDirectory != nullptr) // dest directory mi
				return;

			std::cout << "absSourcePath : " << absSourcePath << std::endl;
			std::cout << "absDestPath : " << absDestPath << std::endl;
			std::cout << "name : " << dest.substr(dest.find_last_of('/') + 1) << std::endl;
			std::cout << "path : " << absDestPath.substr(0, absDestPath.find_last_of('/') - 1) << std::endl;
			std::cout << "linkP : " << absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1) << std::endl;
			std::cout << "Parent of abs :" << Utils::getParentPathOfAbsPath(absDestPath) << std::endl;
			// dest dosyam temiz
			sourceDirectory = Directory::find(shell, Utils::getParentPathOfAbsPath(absSourcePath), nullptr);
			destDirectory = Directory::find(shell, Utils::getParentPathOfAbsPath(absDestPath), nullptr);
			if (destDirectory == nullptr)
				throw invalid_argument("link: cannot create link '" + dest + "': No such file or directory");
			if (sourceDirectory == nullptr){
				destDirectory->addFile(new SymbolicLink(dest.substr(dest.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absDestPath), time(nullptr), sourceFile, source, absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1)));
				return;
			}
			sourceFile = SymbolicLink::find(shell, absSourcePath, nullptr);
			if (sourceFile != nullptr){
				destDirectory->addFile(new SymbolicLink(dest.substr(dest.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absDestPath), time(nullptr), sourceFile, source, absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1)));
				return;
			}
				// destteki dosyayı linkleyip kendimde oluştururum.
			sourceFile = RegularFile::find(shell, absSourcePath, nullptr);
			if (sourceFile != nullptr){
				destDirectory->addFile(new SymbolicLink(dest.substr(dest.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absDestPath), time(nullptr), sourceFile, source, absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1)));
				return;
			}
				// destteki dosyayı linkleyip kendimde oluştururum.
			sourceFile = Directory::find(shell, absSourcePath, nullptr);
			if (sourceFile != nullptr){
				destDirectory->addFile(new SymbolicLink(dest.substr(dest.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absDestPath), time(nullptr), sourceFile, source, absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1)));
				return;
			}
			destDirectory->addFile(new SymbolicLink(dest.substr(dest.find_last_of('/') + 1), Utils::getParentPathOfAbsPath(absDestPath), time(nullptr), sourceFile, source, absSourcePath.substr(0, absSourcePath.find_last_of('/') - 1)));
			// destteki dosyayı linkleyip kendimde oluştururum.
			// null gösteren symbolicLink oluşur.
		} catch (const invalid_argument& e) {
			throw e;
		}
	}
}
