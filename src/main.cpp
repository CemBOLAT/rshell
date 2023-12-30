#include <iostream>
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
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
				Utils::TextEngine::green();
				Utils::TextEngine::bold();
				cout << shell.getPrompt();
				//Utils::TextEngine::reset();
				Utils::TextEngine::magenta();
				string command;
				getline(cin, command);
				Utils::TextEngine::reset();

				shell.execute(command);
			} catch (const exception& e) {
				Utils::TextEngine::bold();
				Utils::TextEngine::red();
				cerr << e.what();
				Utils::TextEngine::reset();
				cout << endl;
			}
		}
		SaveFile::save(shell.getFileSystemPath(), shell);
	} catch (const exception& e) {
		cerr << e.what() << endl;
	}

}
