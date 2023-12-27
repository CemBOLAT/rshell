#ifndef UTILS_HPP

# define UTILS_HPP

# include <string>
# include <fstream>

using namespace std;

namespace Utils {
	std::string trim(const std::string& str);
	std::string getContent(std::ifstream &file);
}

#endif
