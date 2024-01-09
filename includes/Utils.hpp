#ifndef UTILS_HPP

# define UTILS_HPP

# include <string>
# include <vector>
# include <fstream>

class Directory;
class Shell;
class RegularFile;
class SymbolicLink;
class File;

using namespace std;

// Utils class contains functions that are used in multiple classes
namespace Utils {
	std::string		trim(const std::string& str);
	std::string		getContent(std::ifstream &file);
	std::ostream&	printTime(std::ostream& os, struct tm* timeinfo);
	std::string		getData(std::ifstream &file);
	vector<string>	split(const string &str, char delim);
	string			relPathToAbsPath(const Shell &shell, const string &path);
	string			getParentPathOfAbsPath(const string &absPath);
	size_t			getProgramSize(const Shell &Shell);
	void			recheckLinks(Shell &shell, Directory *directory);
	void			printPrompt(const Shell &shell);
}

#endif
