/*
	Shell class is used to store the current state of the shell.
*/
#ifndef SHELL_HPP
# define SHELL_HPP

#include "RegularFile.hpp"
#include "Utils.hpp"

class Directory;

class Shell {
	public:
		Shell();
		Shell(const Shell& shell) = default;
		Shell&	operator=(const Shell& shell) = default;

		~Shell();
		// Getters and setters
		bool			isTerminated() const { return this->isTerminating; }
		void			setTerminated(bool isTerminated) { this->isTerminating = isTerminated; }
		void			setPrompt(const string& prompt) { this->prompt = prompt;}
		string			getPrompt() const { return this->prompt; }
		const string	getFileSystemPath() const { return Shell::fileSystemPath; }
		Directory*		getCurrentDirectory() const { return this->currentDirectory; }
		void			setCurrentDirectory(Directory* directory) { this->currentDirectory = directory; }
		Directory*		getRoot() const { return this->root; }
		size_t			getOsSize() const { return osSize; }
		// Functions that execute commands
		void			execute(string command);
	private:
		static const string	fileSystemPath; // path of the file that stores the file system
		Directory*			root; // root directory
		Directory*			currentDirectory; // current directory
		bool				isTerminating;
		string				prompt; // prompt
		static const size_t	osSize = 1024 * 1024 * 10; // size of the os in bytes
};

#endif
