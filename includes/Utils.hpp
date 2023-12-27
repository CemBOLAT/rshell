#ifndef UTILS_HPP

# define UTILS_HPP

# include <string>
# include <fstream>
# include "Shell.hpp"

class Shell;

using namespace std;

namespace Utils {
	std::string trim(const std::string& str);
	std::string getContent(std::ifstream &file);
	std::ostream& printTime(std::ostream& os, struct tm* timeinfo);

}



#endif
