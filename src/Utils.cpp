#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Shell.hpp"
#include <iomanip>
#include <vector>

using namespace std;
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
	ostream& printTime(ostream& os, struct tm* timeinfo) {
		int day = timeinfo->tm_mday;
		int month = timeinfo->tm_mon + 1;
		int hour = timeinfo->tm_hour;
		int minute = timeinfo->tm_min;

		os << std::setw(2) << std::setfill('0') << day << "/"
			<< std::setw(2) << std::setfill('0') << month << " "
			<< std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute << " ";
		return os;
	}
}
