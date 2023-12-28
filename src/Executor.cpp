#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/UtilsTemplate.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Directory.hpp"
#include <ctime>

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
		Directory* currentDirectory = Shell.getRoot();
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
			regularFile = Utils::findFile<RegularFile>(shell, fileName);
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
	// bağımlıda yok <sebep current da değil bağımlıda olmalı ! >
	void rm(const Shell& shell, const string& fileName) {
		RegularFile *regularFile;
		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		try {
			regularFile = Utils::findFile<RegularFile>(shell,fileName);
			if (regularFile == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
			shell.getCurrentDirectory()->removeFile(regularFile->getName());
		} catch (const runtime_error& e) {
			throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory");
		} catch (const invalid_argument& e) {
			throw e;
		}
	}
}

namespace Executor{
	// bağımlıda çalışmıyor !
	void mkdir(const Shell& shell, const string& fileName) {
		Directory *directory;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		try {
			directory = Utils::findDirectory(shell.getCurrentDirectory(), fileName);
			if (directory != nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists");
			directory = new Directory(fileName, time(nullptr) , shell.getCurrentDirectory()->getPath() + fileName + "/", shell.getCurrentDirectory());
			shell.getCurrentDirectory()->addFile(directory);
		} catch (const runtime_error& e) {
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		} catch (const invalid_argument& e) {
			throw e;
		}

	}
}

namespace Executor {
	void cd(Shell& shell, const string& directoryName) {
		Directory *directory;
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
				//std::cout << "directoryName: " << directoryName << std::endl;
				directory = Utils::findDirectory(shell.getCurrentDirectory(), directoryName);
				if (directory == nullptr)
					throw invalid_argument("cd: " + directoryName + ": No such file or directory");
				shell.setCurrentDirectory(directory);
				//std::cout << shell.getCurrentDirectory()->getPath() << std::endl;
			} catch (const runtime_error& e) {
				throw runtime_error("cd: " + directoryName + ": Not a directory");
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
				std::cout << "\n."<< directory->getPath() << ":" << std::endl;
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
namespace Executor {
	void cp(const Shell& shell, const vector<string>& args) {

	}

}
