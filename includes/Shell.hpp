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

		void			setCurrentDirectoryPath(const string& path);
		string			getCurrentDirectoryPath() const;

		const string	getFileSystemPath() const;

		Directory*		getRoot() const;

		void			execute(string command);

	private:
		static const string	fileSystemPath;
		Directory*			root;
		bool				isTerminating;
		string				prompt;
		string				currentDirectoryPath;

};

#endif
