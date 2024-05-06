// Executor.hpp
// Executor is a namespace that contains functions that execute commands.
#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "Shell.hpp"

namespace Executor {
	void	cat(const Shell& shell, const string& fileName);
	void	rm(const Shell& shell, const string& fileName);
	void	mkdir(const Shell& shell, const string& directoryName);
	void	cd(Shell& shell, const string& directoryName);
	void	ls(const Directory *directory, const Shell &shell, const string& option);
	void	cp(const Shell& shell, const string& source, const string& fileName);
	void	link(const Shell& shell, const string& source, const string& fileName);
	void	clear();
	void	touch(const Shell& shell, const vector<string>& _splitpath);
	void	echo(const Shell& shell, const string &text, const string &fileName);
}

#endif
