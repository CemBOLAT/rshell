#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "Shell.hpp"

namespace Executor {
	void ls(const Shell &Shell);
	void cat(const Shell& shell, const string& fileName);
	void rm(const Shell& shell, const string& fileName);
}

#endif
