#include "../includes/Utils.hpp"
#include "../includes/UtilsTemplate.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Shell.hpp"
#include <iomanip>
#include <vector>
#include <sstream>

// trimleri ayrıntılı bak
using namespace std;
namespace Utils{
	std::vector<std::string> split(const string &str, char delim) {
		std::vector<std::string> elems;
		std::stringstream ss(str);
		std::string item;
		while (std::getline(ss, item, delim)) {
			if (!item.empty())
				elems.push_back(item);
		}
		return elems;
	}
	std::string trim(const std::string& str) {
		size_t first = str.find_first_not_of(" \t\r");
		size_t last = str.find_last_not_of(" \t\r");

		if (first == std::string::npos || last == std::string::npos)
			return "";

		return str.substr(first, last - first + 1);
	}
	std::string getContent(std::ifstream &file) {
		string content;
		// son satırda
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
	std::string getData(std::ifstream &file) {
		string data;
		string line;
		getline(file, line);
		data = line.substr(line.find(" ") + 1, line.size() - 1);
		if (data[data.size() - 1] == static_cast<char>(3)){
			return data;
		}
		data += "\n";
		while (getline(file, line))
		{
			trim(line);
			if (line.empty())
				break;
			data += line;
			if (line[line.size() - 1] == static_cast<char>(3)){
				break;
			}
			data += "\n";

		}
		return data;
	}
	std::string	subParentPath(const std::string& path) {
		size_t pos = path.find_last_of("/");
		if (pos == std::string::npos || pos == 0) {
			return path;
		}

		return path.substr(0, path.find_last_of("/", pos-1)+1);
	}

	Directory*	findDirectory(Directory *root, const std::string &path) {
		Directory*		directory = root;
		vector<string>	directories = Utils::split(path, '/');

		std::cout << "directories.size(): " << directories.size() << std::endl;
		if (path == "/")
			return root;
		else if (directories.size() == 1){
			return root;
		}
		for (size_t i = 0; i < directories.size(); i++)
		{
			//looks like it works
			//std::cout << "directories[" << i << "]: " << directories[i] << std::endl;
			if (directories[i] == ".")
				continue;
			else if (directories[i] == "..")
				directory = directory->getParentDirectory();
			else if (directories[i] == "~" || directories[i] == "/")
				directory = root;
			else
				directory = directory->getDirectory(directories[i]);
			if (directory == nullptr)
				return nullptr;
		}
		return directory;
	}
}
