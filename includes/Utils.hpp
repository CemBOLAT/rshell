#ifndef UTILS_HPP

# define UTILS_HPP

# include <string>
# include <fstream>
# include "Shell.hpp"
# include "RegularFile.hpp"

class Shell;

using namespace std;

namespace Utils {
	std::string		trim(const std::string& str);
	std::string		getContent(std::ifstream &file);
	std::ostream&	printTime(std::ostream& os, struct tm* timeinfo);
	std::string		getData(std::ifstream &file);
	vector<string>	split(const string &str, char delim);
	string			relPathToAbsPath(const Shell &shell, const string &path);
	string			getParentPathOfAbsPath(const string &absPath);
	void			terminate(Directory *directory);
	size_t			getProgramSize(const Directory *directory);
	void			recheckLinks(Shell &shell, Directory *directory);
}



#endif
