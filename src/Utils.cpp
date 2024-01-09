#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Shell.hpp"
#include <iomanip>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <stdexcept>

using namespace std;

namespace
{
	// Precondition: month is between 1 and 12
	// Postcondition: returns the string of month
	string convertMonth(int month)
	{
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
	vector<string> split(const string &str, char delim)
	{
		std::vector<std::string> elems;
		std::stringstream ss(str);
		std::string item;
		while (std::getline(ss, item, delim))
		{
			if (!item.empty())
				elems.push_back(item);
		}
		return elems;
	}
	// Precondition: str is a string
	// Postcondition: returns the trimmed string
	string trim(const std::string &str)
	{
		size_t first = str.find_first_not_of(" \t\r");
		size_t last = str.find_last_not_of(" \t\r");

		if (first == std::string::npos || last == std::string::npos)
			return "";

		return str.substr(first, last - first + 1);
	}
	// Precondition : file is a ifstream
	// Postcondition: returns the content of file (without spaces)
	string getContent(std::ifstream &file)
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

		os << std::right << std::setw(3) << std::setfill('0') << convertMonth(month) << " "
		   << std::right << std::setw(2) << std::setfill('0') << day << " "
		   << std::right << std::setw(2) << std::setfill('0') << hour << ":"
		   << std::right << std::setw(2) << std::setfill('0') << minute << " ";
		return os;
	}
	// Precondition: file is a ifstream
	// Postcondition: returns the data of file
	string getData(std::ifstream &file)
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
			if (!line.empty() && line[line.size() - 1] == static_cast<char>(3))
			{
				break;
			}
			data += "\n";
		}
		line += static_cast<char>(3);
		return data;
	}
	// Precondition: path is a string
	// Postcondition: returns the absolute path of path
	string relPathToAbsPath(const Shell &shell, const std::string &path)
	{
		string relPathToAbsPath;
		vector<string> relPathToAbsPathSplit;
		vector<string> relPathToAbsPathSplitTmp;
		if (path[0] == '/')
			relPathToAbsPathSplit = Utils::split(path, '/');
		else
		{
			relPathToAbsPathSplit = Utils::split(shell.getCurrentDirectory()->getOwnFilesPath() + "/" + path, '/');
		}
		if (relPathToAbsPathSplit.size() == 0)
			return "/";
		for (auto &path : relPathToAbsPathSplit)
		{
			// std::cout << path << std::endl;
			if (path == ".")
				continue;
			else if (path == "..")
			{
				if (relPathToAbsPathSplitTmp.size() > 0)
					relPathToAbsPathSplitTmp.pop_back();
			}
			else
				relPathToAbsPathSplitTmp.push_back(path);
		}
		relPathToAbsPath = "";
		for (auto &path : relPathToAbsPathSplitTmp)
		{
			relPathToAbsPath += "/" + path;
		}
		if (relPathToAbsPath == "")
			relPathToAbsPath = "/";
		return relPathToAbsPath;
	}

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
	// Postcondition: returns the size of directory recursively
	// Note: size of directory is the sum of size of its files and its subdirectories recursively + 15 is the size of saving system information
	size_t getProgramSize(const Shell &Shell)
	{
		struct stat st;
		stat(Shell.getFileSystemPath().c_str(), &st);
		return st.st_size;
	}
}

namespace Utils
{
	// Precondition: shell is a Shell
	// Postcondition: rechecks the links of shell recursively because of the possibility of deleting linked files or creating new files for linking to them
	void recheckLinks(Shell &shell, Directory *directory)
	{
		for (auto it = directory->getFiles().begin(); it != directory->getFiles().end(); ++it)
		{
			if (dynamic_cast<SymbolicLink *>(*it) != nullptr)
			{ // if file is a symbolic link
				SymbolicLink *link = dynamic_cast<SymbolicLink *>(*it);
				File *linkFile = File::find<File>(shell, link->getLinkedPath() + "/" + link->getLinkedName());
				link->setLink(linkFile);
			}
			else if (dynamic_cast<Directory *>(*it) != nullptr)
			{ // if file is a directory then recursive call
				Directory *dir = dynamic_cast<Directory *>(*it);
				recheckLinks(shell, dir);
			}
		}
	}
}

namespace Utils {
	// prints the prompt properly
	void printPrompt(const Shell &shell) {
		Utils::TextEngine::green();
		Utils::TextEngine::bold();
		cout << "cemalBolat@C++0S:";
		Utils::TextEngine::reset();
		Utils::TextEngine::bold();
		Utils::TextEngine::blue();
		cout << shell.getPrompt();
		Utils::TextEngine::magenta();
	}
}
