#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Shell.hpp"
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;

namespace {
	// Precondition: month is between 1 and 12
	// Postcondition: returns the string of month
	string convertMonth(int month){
		switch (month)
		{
		case 1: return "Jan";
		case 2: return "Feb";
		case 3: return "Mar";
		case 4: return "Apr";
		case 5: return "May";
		case 6: return "Jun";
		case 7: return "Jul";
		case 8: return "Aug";
		case 9: return "Sep";
		case 10: return "Oct";
		case 11: return "Nov";
		default: return "Dec";
		}
	}
}

namespace Utils
{
	// Precondition: str is a string
	// Postcondition: returns the vector of splitted string by delim
	vector<string>	split(const string &str, char delim)
	{
		std::vector<std::string>	elems;
		std::stringstream			ss(str);
		std::string					item;
		while (std::getline(ss, item, delim))
		{
			if (!item.empty())
				elems.push_back(item);
		}
		return elems;
	}
	// Precondition: str is a string
	// Postcondition: returns the trimmed string
	string	trim(const std::string &str)
	{
		size_t first = str.find_first_not_of(" \t\r");
		size_t last = str.find_last_not_of(" \t\r");

		if (first == std::string::npos || last == std::string::npos)
			return "";

		return str.substr(first, last - first + 1);
	}
	// Precondition : file is a ifstream
	// Postcondition: returns the content of file (without spaces)
	string	getContent(std::ifstream &file)
	{
		string content;
		getline(file, content);
		content = trim(content);
		if (content.empty())
			throw runtime_error("File content is not configrated!");
		return content.substr(content.find(" ") + 1, content.size() - 1);
	}
	// Precondition: timeinfo is a struct tm
	// Postcondition: prints the timeinfo to os
	ostream &printTime(ostream &os, struct tm *timeinfo)
	{
		int day = timeinfo->tm_mday;
		int month = timeinfo->tm_mon + 1;
		int hour = timeinfo->tm_hour;
		int minute = timeinfo->tm_min;

		os << std::setw(3) << std::setfill('0') << convertMonth(month) << " "
			<< std::setw(2) << std::setfill('0') << day << " "
			<< std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute << " ";
		return os;
	}
	// Precondition: file is a ifstream
	// Postcondition: returns the data of file
	string	getData(std::ifstream &file)
	{
		string data;
		string line;
		getline(file, line);
		data = line.substr(line.find(" ") + 1, line.size() - 1);
		if (data[data.size() - 1] == static_cast<char>(3))
		{
			return data;
		}
		data += "\n";
		while (getline(file, line))
		{
			trim(line);
			data += line;
			if (line[line.size() - 1] == static_cast<char>(3))
			{
				break;
			}
			data += "\n";
		}
		line += static_cast<char>(3) + '\0';
		return data;
	}
	// Precondition: path is a string
	// Postcondition: returns the absolute path of path
	string	relPathToAbsPath(const Shell &shell, const std::string &path)
	{
		if (path[0] == '/'){
			return path;
		}
		if (shell.getCurrentDirectory() == shell.getRoot()){ // current directory is root
			if (path[0] == '.' && path[1] == '.'){ // special condtion for parent directory of root (../ is root)
				return "/" + path.substr(3, path.size() - 1);
			}
			else if (path[0] == '.'){ // path for current directory of root (./ is root)
				return "/" + path.substr(2, path.size() - 1);
			}
			else{ // path for child directory of root
				return "/" + path;
			}
		}
		if ((path[0] == '.' && path[1] == '/') ||  path[0] != '.'){ // path for child directory of current directory
			if (path[0] == '.')
				return shell.getCurrentDirectory()->getOwnFilesPath() + path.substr(2, path.size() - 1);
			return shell.getCurrentDirectory()->getOwnFilesPath() + path;
		}
		if (path[0] == '.' && path[1] == '.' && path[2] == '/'){ // path for parent directory of current directory
			return shell.getCurrentDirectory()->getParentDirectory()->getOwnFilesPath() + path.substr(3, path.size() - 1);
		}
		return "";
	}
	// Precondition: path is a string
	// Postcondition: returns the parent path of path
	string getParentPathOfAbsPath(const string &absPath)
	{
		size_t found = absPath.find_last_of('/');
		if (found != std::string::npos)
		{
			if (found == 0)
				return "/";
			return absPath.substr(0, found);
		}
		return "/";
	}
}

namespace Utils
{
	// Precondition: directory is a Directory
	// Postcondition: deletes the directory and its files recursively from memory for preventing memory leak
	void terminate(Directory *directory)
	{
		for (auto file : directory->getFiles())
		{
			if (dynamic_cast<Directory *>(file))
			{
				terminate(dynamic_cast<Directory *>(file)); // recursive call
			}
			else
			{
				delete file;
				file = nullptr;
			}
		}
		delete directory;
		directory = nullptr;
	}
} // namespace Utils


namespace Utils
{
	// Precondition: directory is a Directory
	// Postcondition: returns the size of directory recursively
	// Note: size of directory is the sum of size of its files and its subdirectories recursively + 15 is the size of saving system information
	size_t getProgramSize(const Directory *directory)
	{
		size_t size = 0;
		for (auto file : directory->getFiles())
		{
			if (dynamic_cast<Directory *>(file))
			{
				size += getProgramSize(dynamic_cast<Directory *>(file)) + 15;
			}
			else if (dynamic_cast<RegularFile *>(file))
			{
				size += dynamic_cast<RegularFile *>(file)->getSizeBytes() + 15;
			}
			else
			{
				size += 15;
			}
		}
		return size;
	}
} // namespace Utils


namespace Utils {
	// Precondition: shell is a Shell
	// Postcondition: rechecks the links of shell recursively because of the possibility of deleting linked files or creating new files for linking to them
	void recheckLinks(Shell &shell, Directory *directory){
		for (auto &file : directory->getFiles()){
			//std::cout << file->getName() << std::endl;
			if (dynamic_cast<SymbolicLink *>(file) != nullptr){ // if file is a symbolic link
				SymbolicLink *link = dynamic_cast<SymbolicLink *>(file);
				if (link->getLink() == nullptr){
					RegularFile		*linkFile = File::find<RegularFile>(shell, link->getLinkedPath());
					Directory		*linkDirectory = File::find<Directory>(shell, link->getLinkedPath());
					if (linkFile != nullptr)
						link->setLink(linkFile);
					else if (linkDirectory != nullptr)
						link->setLink(linkDirectory);
				}
			}
			else if (dynamic_cast<Directory *>(file) != nullptr){ // if file is a directory then recursive call
				Directory *dir = dynamic_cast<Directory *>(file);
				recheckLinks(shell, dir);
			}
		}
	}
}
