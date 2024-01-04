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

		~Shell() { Utils::terminate(this->root); }

		bool			isTerminated() const { return this->isTerminating; }
		void			setTerminated(bool isTerminated) { this->isTerminating = isTerminated; }

		void			setPrompt(const string& prompt) { this->prompt = prompt;}
		string			getPrompt() const { return this->prompt; }

		const string	getFileSystemPath() const { return Shell::fileSystemPath; }

		Directory*		getCurrentDirectory() const { return this->currentDirectory; }
		void			setCurrentDirectory(Directory* directory) { this->currentDirectory = directory; }

		Directory*		getRoot() const { return this->root; }
		void			execute(string command);

		size_t			getOsSize() const { return osSize; }

	private:
		static const string	fileSystemPath;
		Directory*			root;
		Directory*			currentDirectory;
		bool				isTerminating;
		string				prompt;
		static const size_t	osSize = 1024 * 1024 * 10;
};

//# include "../src/Template.cpp"

#endif
