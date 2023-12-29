#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Directory.hpp"
#include <ctime>
#include <sys/stat.h>

namespace {
	void listOnlyCurrentDirectory(ostream &os, const Shell& shell) {
		time_t rawtime = shell.getRoot()->getTime();
		struct tm* timeinfo = std::localtime(&rawtime);

		os << "D " << "." << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}

	void listSpecialDirectories(ostream &os, const Shell& shell) {
		time_t rawtime = shell.getCurrentDirectory()->getTime();
		struct tm* timeinfo = std::localtime(&rawtime);

		os << "D " << "." << " "; // hata var düzelecezzz (zaman sorunu !)
		Utils::printTime(os, timeinfo) << std::endl;

		os << "D " << ".." << " ";
		Utils::printTime(os, timeinfo) << std::endl;
	}
}

namespace Executor {

	void ls(const Shell& Shell) {
		Directory* currentDirectory = Shell.getCurrentDirectory();
		if (Shell.getCurrentDirectory()->getPath() != "/") {
			currentDirectory = Shell.getCurrentDirectory();
			listSpecialDirectories(cout, Shell);
		}
		else {
			listOnlyCurrentDirectory(cout, Shell);
		}
		vector<File*> files = currentDirectory->getFiles();
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				Directory* directory = dynamic_cast<Directory*>(file);
				cout << *directory << endl;
			} else if (dynamic_cast<RegularFile*>(file)) {
				RegularFile* regularFile = dynamic_cast<RegularFile*>(file);
				cout << *regularFile << endl;
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
			regularFile = Utils::findRegularFile(shell, fileName);
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
			regularFile = Utils::findRegularFile(shell, fileName);
			directory = Utils::findDirectory(shell, fileName.substr(0, fileName.find_last_of('/')));
			//std::cout << fileName << " AA" << std::endl;

			if (regularFile == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			directory->removeFile(regularFile->getName());
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
			directory = Utils::findDirectory(shell, fileName);
			parentDirectory = Utils::findDirectory(shell, fileName.substr(0, fileName.find_last_of('/')));
			if (directory != nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists");
			directory = new Directory(fileName.substr(fileName.find_last_of("/") + 1, fileName.size() - 1), time(nullptr), shell.getCurrentDirectory()->getPath(), shell.getCurrentDirectory());
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
			directory = Utils::findDirectory(shell, directoryName);
			if (directory == nullptr)
				throw invalid_argument("cd: " + directoryName + ": No such file or directory");
			shell.setCurrentDirectory(directory);
		} catch (const runtime_error& e) {
			throw e;
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
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				Directory* directory = dynamic_cast<Directory*>(file);
				cout << *directory << endl;
			} else if (dynamic_cast<RegularFile*>(file)) {
				RegularFile* regularFile = dynamic_cast<RegularFile*>(file);
				cout << *regularFile << endl;
			}
			//else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			//}
		}
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				Directory* directory = dynamic_cast<Directory*>(file);
				std::cout << "\n."<< directory->getPath() + directory->getName() << ":" << std::endl;
				listRecursive(directory); //sonra karar verirsin
			}
		}
	}
}


namespace Executor {
	void lsRecursive(const Directory *directory, const Shell& Shell) {

		if (directory->getPath() != "/")
		{
			listSpecialDirectories(cout, Shell);
		}
		else
		{
			listOnlyCurrentDirectory(cout, Shell);
		}
		listRecursive(directory);
	}
}

// 1. cp cannot create a file in a directory that does not exist
// 2. cp can swap the contents of two files so no need to create new file when the file is exist at that directory
// kendi pc'inde source file !
// namespace Executor {
// 	void cp(const Shell& shell, const string &source, const string &fileName){

// 		struct stat	sourceStat;

// 		if (source.empty() || fileName.empty())
// 			throw runtime_error("cp: missing operand");
// 		//else if (source == "." || source == ".." || fileName == "." || fileName == "..")
// 		//	throw runtime_error("cp: cannot copy '" + source + "': Is a directory");


// 	}

// }
