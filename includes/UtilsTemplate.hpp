#ifndef UTILSTEMPLATE_HPP

# define UTILSTEMPLATE_HPP

# include <string>
# include <fstream>
# include "Shell.hpp"

class Shell;

using namespace std;

namespace Utils {
	template <typename T>
	T *findFile(const Shell& shell, const string& fileName, const string& currentDirectoryPath);
}

# include "../src/UtilsTemplate.cpp"



#endif
