#include "../includes/Utils.hpp"
#include "../includes/RegularFile.hpp"
#include <iomanip>
#include <vector>
#include <sstream>

// trimleri ayrıntılı bak
using namespace std;
namespace Utils
{
	std::vector<std::string> split(const string &str, char delim)
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
	std::string trim(const std::string &str)
	{
		size_t first = str.find_first_not_of(" \t\r");
		size_t last = str.find_last_not_of(" \t\r");

		if (first == std::string::npos || last == std::string::npos)
			return "";

		return str.substr(first, last - first + 1);
	}
	std::string getContent(std::ifstream &file)
	{
		string content;
		// son satırda
		getline(file, content);
		content = trim(content);
		if (content.empty())
			throw runtime_error("File content is not configrated!");
		return content.substr(content.find(" ") + 1, content.size() - 1);
	}
	ostream &printTime(ostream &os, struct tm *timeinfo)
	{
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
	std::string getData(std::ifstream &file)
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
			if (line.empty())
				break;
			data += line;
			if (line[line.size() - 1] == static_cast<char>(3))
			{
				break;
			}
			data += "\n";
		}
		return data;
	}

	string relPathToAbsPath(const Shell &shell, const string &path)
	{
		if (path[0] == '/'){
			return path;
		}
		if (path[0] == '.')
		{
			if (path[1] == '.')
			{
				if (shell.getCurrentDirectory()->getParentDirectory() == nullptr)
					return shell.getCurrentDirectory()->getPath() + path.substr(2);
				return shell.getCurrentDirectory()->getParentDirectory()->getPath() + shell.getCurrentDirectory()->getParentDirectory()->getName() + path.substr(2);
			}
			if (shell.getCurrentDirectory()->getParentDirectory() == nullptr)
				return shell.getCurrentDirectory()->getPath() + shell.getCurrentDirectory()->getName() + "/" + path.substr(2);
			return shell.getCurrentDirectory()->getParentDirectory()->getPath() + shell.getCurrentDirectory()->getName() + "/" + path.substr(2);
		}
		return shell.getCurrentDirectory()->getPath() + shell.getCurrentDirectory()->getName() + "/" + path;
	}
	Directory *findDirTraverse(Directory *directory, const vector<string> &path)
	{
		for (auto file : directory->getFiles())
		{
			if (file->getName() == path[0])
			{
				if (path.size() == 1)
				{
					return dynamic_cast<Directory *>(file);
				}
				else
				{
					return findDirTraverse(dynamic_cast<Directory *>(file), vector<string>(path.begin() + 1, path.end()));
				}
			} // throw olabilir
		}
		return nullptr;
	}
	Directory *findDirectory(const Shell &shell, const std::string &path)
	{
		string absPath = relPathToAbsPath(shell, path);
 		vector<string> paths = split(absPath, '/');
		std::cout << "absPath: " << absPath << std::endl;
		std::cout << "path  : " << path << std::endl;
		for (auto path : paths)
		 	std::cout << path << std::endl;
		Directory *dir = findDirTraverse(shell.getRoot(), paths);
		if (dir == nullptr)
			throw invalid_argument("cd: " + path + ": No such file or directory");
		//std::cout << *dir << std::endl;
		// cout << "000000" << endl;
		return dir;
	}
	RegularFile *findRegFileTraverse(Directory *directory, const vector<string> &path)
	{
		for (auto file : directory->getFiles())
		{
			if (file->getName() == path[0])
			{
				if (path.size() == 1)
				{
					return dynamic_cast<RegularFile *>(file);
				}
				else
				{
					return findRegFileTraverse(dynamic_cast<Directory *>(file), vector<string>(path.begin() + 1, path.end()));
				}
			} // throw olabilir
		}
		return nullptr;
	}
	RegularFile *findRegularFile(const Shell &shell, const std::string &path)
	{
		string absPath = relPathToAbsPath(shell, path);
		vector<string> paths = split(absPath, '/');
		// std::cout << "absPath: " << absPath << std::endl;
		// std::cout << "path  : " << path << std::endl;
		// for (auto path : paths)
		// 	std::cout << path << std::endl;
		RegularFile *regFile = findRegFileTraverse(shell.getRoot(), paths);
		//std::cout << *regFile << std::endl;
		// cout << "000000" << endl;
		return regFile;
	}
}
