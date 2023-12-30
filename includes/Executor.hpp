#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "Shell.hpp"

namespace Executor {
	void	ls(const Shell &Shell);
	void	cat(const Shell& shell, const string& fileName);
	void	rm(const Shell& shell, const string& fileName);
	void	mkdir(const Shell& shell, const string& directoryName);
	void	cd(Shell& shell, const string& directoryName);
	void	lsRecursive(const Directory *directory, const Shell &shell);
	void	cp(const Shell& shell, const string& source, const string& fileName);
	//void	link(const Shell& shell, const string& source, const string& fileName);
}

#endif
