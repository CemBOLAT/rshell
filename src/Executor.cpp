#include "../includes/Executor.hpp"
#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Directory.hpp"




namespace Executor {
	// ls için . ve .. yok onları da ekle yeri gelince
	void ls(const Shell& Shell) {
		Directory* currentDirectory = Shell.getRoot();
		string currentDirectoryPath = Shell.getCurrentDirectoryPath();
		if (currentDirectoryPath != "/") {
			currentDirectory = currentDirectory->getDirectory(currentDirectoryPath);
		}
		vector<File*> files = currentDirectory->getFiles();
		for (File* file : files) {
			if (dynamic_cast<Directory*>(file)) {
				Directory* directory = dynamic_cast<Directory*>(file);
				cout << *directory;
			} else if (dynamic_cast<RegularFile*>(file)) {
				RegularFile* regularFile = dynamic_cast<RegularFile*>(file);
				cout << *regularFile;
			}
			//else if (dynamic_cast<SymbolicLink*>(file)) {
			//	SymbolicLink* symbolicLink = dynamic_cast<SymbolicLink*>(file);
			//	cout << *symbolicLink;
			//}
		}
	}
}

