#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Directory.hpp"
#include "./Template.cpp"
#include <ctime>
#include <sys/stat.h>
#include <fstream>

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

		os << "D " << std::setw(maxNameLength) << std::setfill(' ') << "." << " ";
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
			if (dynamic_cast<Directory*>(file)) {
				file->print(cout, maxNameLength);
			} else if (dynamic_cast<RegularFile*>(file)) {
				file->print(cout, maxNameLength);
			}
			//else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			//}
		}
	}
}

namespace Executor {
	void cat(const Shell& shell, const string& fileName) {
		RegularFile *regularFile;
		if (fileName.empty())
			throw runtime_error("cat: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("cat: " + fileName + ": Is a directory");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			regularFile = Utils::findRegularFile(shell, absPath);
			if (regularFile == nullptr)
				throw invalid_argument("cat: " + fileName + ": No such file or directory");
		} catch (const runtime_error& e) {
			throw runtime_error("cat: " + fileName + ": Is a directoryr");
		} catch (const invalid_argument& e) {
			throw e;
		}
		cout << regularFile->getData() << endl;
	}
}

namespace Executor {
	void rm(const Shell& shell, const string& fileName) {
		RegularFile	*regularFile;
		Directory	*directory;
		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			regularFile = Utils::findRegularFile(shell, absPath);
			directory = Utils::findDirectory(shell, absPath.substr(0, absPath.find_last_of('/')));
			//std::cout << fileName << " AA" << std::endl;

			if (regularFile == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			directory->removeFile<RegularFile>(regularFile->getName());
		} catch (const runtime_error& e) {
			throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory");
		} catch (const invalid_argument& e) {
			throw e;
		}
	}
}

namespace Executor{
	void mkdir(const Shell& shell, const string& fileName) {
		Directory *directory;
		Directory *parentDirectory;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		try {
			string absPath = Utils::relPathToAbsPath(shell, fileName);
			directory = Utils::findDirectory(shell, absPath);
			std::cout << "abs BB : " << Utils::relPathToAbsPath(shell, fileName) << std::endl;
			string pPath = Utils::getParentPathOfAbsPath(Utils::relPathToAbsPath(shell, fileName));
			std::cout << "abs AA : " << pPath << std::endl;
			parentDirectory = Utils::findDirectory(shell, pPath);
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
		Directory *directory;
		if (directoryName.empty())
			throw runtime_error("cd: missing operand");
		else if (directoryName == "." || directoryName == "..")
			throw runtime_error("cd: " + directoryName + ": Not a directory");
		try {
			string pPath = Utils::relPathToAbsPath(shell, directoryName);
			directory = Utils::findDirectory(shell, pPath);
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

namespace {
	void listRecursive(const Directory *directory){
		vector<File *> files = directory->getFiles();
		size_t maxNameLength = 0;
		for (auto file : files){
			if (file->getName().size() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				file->print(cout, maxNameLength);

			} else if (dynamic_cast<RegularFile*>(file)) {
				file->print(cout, maxNameLength);
			}
			//else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			//}
		}
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				Directory* directory = dynamic_cast<Directory*>(file);
				cout << "\n";
				Utils::TextEngine::greenBackground();
				cout << "." << directory->getPath() + directory->getName() << ":";
				Utils::TextEngine::reset();
				cout << endl;
				// (. ve ..) var untuma
				listRecursive(directory); //sonra karar verirsin
			}
		}
	}
}


namespace Executor {
	void lsRecursive(const Directory *directory, const Shell& Shell) {
		Directory *currentDirectory = Shell.getCurrentDirectory();
		vector<File *> files = currentDirectory->getFiles();
		size_t maxNameLength = 0;
		for (File *file : files)
		{
			if (file->getName().length() > maxNameLength)
				maxNameLength = file->getName().length();
		}
		if (directory->getPath() != "/")
		{
			listSpecialDirectories(cout, Shell, maxNameLength);
		}
		else
		{
			listOnlyCurrentDirectory(cout, Shell, maxNameLength);
		}
		listRecursive(directory);
	}
}


namespace {
	RegularFile *copyRegularFile(const string &source, const string &fileName, const Shell& shell, const struct stat &sourceStat){
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
		regularFile = new RegularFile(fileName, data.size(), sourceStat.st_mtime, data, shell.getCurrentDirectory()->getOwnFilesPath());
		return regularFile;
	}
}

// namespace {
// 	Directory *copyDirectory(const string &source, const string &fileName, const Shell& shell, const struct stat &sourceStat){
// 		Directory		*directory;

// 	}
// }

// 1. cp cannot create a file in a directory that does not exist
// 2. cp can swap the contents of two files so no need to create new file when the file is exist at that directory
// kendi pc'inde source file !
namespace Executor {
	void cp(const Shell& shell, const string &source, const string &fileName){

		struct stat	sourceStat;

		if (source.empty() || fileName.empty())
			throw runtime_error("cp: missing operand");
		if (stat(source.c_str(), &sourceStat) != 0)
		{
			throw std::runtime_error("cp: source file '" + source + "' does not exist");
		}
		RegularFile *regularFile = Utils::findRegularFile(shell, fileName);
		Directory *directory = Utils::findDirectory(shell, fileName);
		if (regularFile == nullptr && S_ISREG(sourceStat.st_mode)){
			//std::cout << "buraya girdi" << std::endl;
			regularFile = copyRegularFile(source, fileName, shell, sourceStat);
			shell.getCurrentDirectory()->addFile(regularFile);
		}
		else if (directory == nullptr && S_ISDIR(sourceStat.st_mode)){ // reküfsif
			//directory = copyDirectory(source, fileName, shell, sourceStat);
		}
		else if (directory && S_ISDIR(sourceStat.st_mode)){ // rekürsif üstüne yaz
			throw std::runtime_error("cp: cannot overwrite directory '" + fileName + "' with non-directory");
		}
		else if (regularFile && S_ISREG(sourceStat.st_mode)){
			//std::cout << "buraya gir11di" << std::endl;
			shell.getCurrentDirectory()->removeFile<RegularFile>(fileName);
			regularFile = copyRegularFile(source, fileName, shell, sourceStat);
			shell.getCurrentDirectory()->addFile(regularFile);
		}
	}

}
