#include <iostream>
#include "../includes/Shell.hpp"
#include "../includes/LoadFile.hpp"
#include "../includes/SaveFile.hpp"


// FileSystem data için tek satır tutuyor onu düzeltirsin! #Dolar mantığı önemli 
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
		SaveFile::save(shell.getFileSystemPath(), shell);
	} catch (const exception& e) {
		cerr << e.what() << endl;
	}

}
