#include <iostream>
#include "../includes/Shell.hpp"
#include "../includes/LoadFile.hpp"


// FileSystem data için tek satır tutuyor onu düzeltirsin!
// filesystem dosyasında dosya zamanı yok onu düzeltirsin!
int main(){

	try {
		Shell shell;
		LoadFile::load(shell.getFileSystemPath(), shell);
		while (!shell.isTerminated()) {
			try {
				cout << shell.getPrompt();
				string command;
				getline(cin, command);
				shell.execute(command);
			} catch (const exception& e) {
				cerr << e.what() << endl;
			}
		}
	} catch (const exception& e) {
		cerr << e.what() << endl;
	}

}
