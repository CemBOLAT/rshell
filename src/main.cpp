#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/LoadFile.hpp"
#include "../includes/Shell.hpp"
#include <iostream>

int main(){

	try {
		Shell	shell;
		LoadFile::load(shell.getFileSystemPath(), shell); // loading file system
		while (!shell.isTerminated()) {
			try {
				Utils::printPrompt(shell); // printing prompt
				string	command;
				getline(cin, command); // getting command
				Utils::TextEngine::reset();
				shell.execute(command); // executing command
			} catch (const exception& e) {
				Utils::TextEngine::bold();
				Utils::TextEngine::red();
				cerr << e.what();
				Utils::TextEngine::reset();
				cout << endl;
			}
		}
	} catch (const exception& e) {
		Utils::TextEngine::bold();
		Utils::TextEngine::red();
		cerr << e.what();
		Utils::TextEngine::reset();
		cout << endl;
	}
}
