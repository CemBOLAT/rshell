#ifndef SHELL_HPP
# define SHELL_HPP

#include "Directory.hpp"

class Shell {
	public:
		Shell();
		Shell(const Shell& shell) = default;
		Shell&	operator=(const Shell& shell) = default;
		virtual	~Shell();

		bool			isTerminated() const;
		void			setTerminated(bool isTerminated);

		void			setPrompt(const string& prompt);
		string			getPrompt() const;

		const string	getFileSystemPath() const;

		Directory*		getCurrentDirectory() const;
		void			setCurrentDirectory(Directory* directory);

		Directory*		getRoot() const;
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

#endif
