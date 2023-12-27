#include "../includes/Utils.hpp"

namespace Utils{
	std::string trim(const std::string& str) {
		size_t first = str.find_first_not_of(" \t\n\r");
		size_t last = str.find_last_not_of(" \t\n\r");

		if (first == std::string::npos || last == std::string::npos)
			return "";

		return str.substr(first, last - first + 1);
	}
	std::string getContent(std::ifstream &file) {
		string content;
		getline(file, content);
		content = trim(content);
		if (content.empty())
			throw runtime_error("File content is not configrated!");
		return content.substr(content.find(" ") + 1, content.size() - 1);
	}
}
