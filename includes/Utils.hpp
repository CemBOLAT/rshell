#ifndef UTILS_HPP

# define UTILS_HPP

# include <string>
# include <fstream>
# include "Shell.hpp"

class Shell;

using namespace std;

namespace Utils {
	std::string		trim(const std::string& str);
	std::string		getContent(std::ifstream &file);
	std::ostream&	printTime(std::ostream& os, struct tm* timeinfo);
	std::string		getData(std::ifstream &file);
	std::string		subParentPath(const std::string& path);
	Directory		*findDirectory(Directory *root, const std::string &path);
	vector<string>	split(const string &str, char delim);
}



#endif
