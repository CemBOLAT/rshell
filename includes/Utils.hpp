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
	string relPathToAbsPath(const Shell &shell, const string &path);

	Directory		*findDirectory(const Shell &shell, const std::string &path);
	Directory		*findDirTraverse(Directory *directory, const vector<string> &path);
	RegularFile		*findRegularFile(const Shell &shell, const std::string &path);
	RegularFile		*findRegFileTraverse(Directory *directory, const vector<string> &path);
	string getParentPathOfAbsPath(const string &absPath);
}



#endif
