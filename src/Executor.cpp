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

	//void listSpecialDirectories(const string& currentDirectoryPath, const Shell& shell) {
	//	cout << currentDirectoryPath << ":" << endl;
}

namespace Executor {
	// ls için . ve .. yok onları da ekle yeri gelince
	void ls(const Shell& Shell) {
		Directory* currentDirectory = Shell.getRoot();
		string currentDirectoryPath = Shell.getCurrentDirectoryPath();
		if (currentDirectoryPath != "/") {
			currentDirectory = currentDirectory->getDirectory(currentDirectoryPath);
			//listSpecialDirectories(currentDirectoryPath, Shell); bagacaz
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
		string currentDirectoryPath;
		RegularFile *regularFile;
		if (fileName.empty())
			throw runtime_error("cat: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("cat: " + fileName + ": Is a directory");
		currentDirectoryPath = shell.getCurrentDirectoryPath();
		try {
			regularFile = Utils::findFile<RegularFile>(shell,
			fileName, currentDirectoryPath);
		} catch (const runtime_error& e) {
			throw runtime_error("cat: " + fileName + ": Is a directoryr");
		} catch (const invalid_argument& e) {
			throw runtime_error("cat: " + fileName + ": No such file or directory");
		}
		cout << regularFile->getData() << endl;
	}
}

namespace Executor {
	void rm(const Shell& shell, const string& fileName) {
		string currentDirectoryPath;
		RegularFile *regularFile;
		if (fileName.empty())
			throw runtime_error("rm: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("rm: " + fileName + ": Is a directory");
		currentDirectoryPath = shell.getCurrentDirectoryPath();
		try {
			regularFile = Utils::findFile<RegularFile>(shell,
			fileName, currentDirectoryPath);
			if (regularFile == nullptr)
				throw invalid_argument("rm: cannot remove '" + fileName + "': No such file or directory");
		} catch (const runtime_error& e) {
			throw runtime_error("rm: cannot remove '" + fileName + "': Is a directory");
		} catch (const invalid_argument& e) {
			throw e;
		}
		shell.getCurrentDirectory()->removeFile(regularFile->getName());
	}
}

namespace Executor{
	void mkdir(const Shell& shell, const string& fileName) {
		string currentDirectoryPath;
		Directory *directory;
		if (fileName.empty())
			throw runtime_error("mkdir: missing operand");
		else if (fileName == "." || fileName == "..")
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		currentDirectoryPath = shell.getCurrentDirectoryPath();
		try {
			directory = Utils::findFile<Directory>(shell, fileName, currentDirectoryPath);
			if (directory != nullptr)
				throw invalid_argument("mkdir: cannot create directory '" + fileName + "': File exists");
			directory = new Directory(fileName, time(nullptr) , shell.getCurrentDirectoryPath() + fileName + "/");
			shell.getCurrentDirectory()->addFile(directory);
		} catch (const runtime_error& e) {
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': File exists");
		} catch (const invalid_argument& e) {
			throw runtime_error("mkdir: cannot create directory '" + fileName + "': No such file or directory");
		}

	}
}
// kaldığım yer
//namespace Executor {
//	void cd(const Shell& shell, const string& directoryName) {
//		string currentDirectoryPath;
//		Directory *directory;
//		if (directoryName.empty())
//			throw runtime_error("cd: missing operand");
//	}
//}
