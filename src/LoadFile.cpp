#include "../includes/LoadFile.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Utils.hpp"
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <stdexcept>

using namespace std;

// Precondition: program wants to load regular file
// Postcondition: regular file is loaded
void LoadFile::addRegularFile(Shell &shell, ifstream &file)
{
	string name, path, time, data;
	Directory *directory = shell.getRoot();
	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);
	data = Utils::getData(file);

	time_t time_t_time = stoi(time);

	if (path == "/")
	{
		directory = shell.getRoot();
		directory->addFile(new RegularFile(name, data.size(), time_t_time, data, path));
		return;
	}
	directory = File::find<Directory>(shell, path);

	if (directory == nullptr)
		throw runtime_error("Filesystem may not be configrated ! ");
	directory->addFile(new RegularFile(name, data.size(), time_t_time, data, path));
}

// Precondition: program wants to load directory
// Postcondition: directory is loaded
void LoadFile::addDirectory(Shell &shell, ifstream &file)
{
	string name, path, time;
	Directory *parentDirectory;

	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);

	time_t time_t_time = stoi(time);

	if (path == "/")
	{
		parentDirectory = shell.getRoot();
		parentDirectory->addFile(new Directory(name, time_t_time, path, parentDirectory));
	}
	else
	{
		parentDirectory = File::find<Directory>(shell, path);
		if (parentDirectory == nullptr)
			throw runtime_error("Directory coould not be found.");
		parentDirectory->addFile(new Directory(name, time_t_time, path, parentDirectory));
	}
}

// Precondition: program wants to load symbolic link
// Postcondition: symbolic link is loaded
void LoadFile::addSymbolicLink(Shell &shell, ifstream &file)
{
	string name, path, time, linkPath, linkerName;
	File *link = nullptr;
	Directory *directory = nullptr;

	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);
	linkPath = Utils::getContent(file);
	linkerName = Utils::getContent(file);

	time_t time_t_time = stoi(time);

	link = File::find<File>(shell, linkPath + linkerName);
	directory = File::find<Directory>(shell, path);

	if (directory == nullptr)
		throw runtime_error("Filesystem file is broken"); // not configrated
	directory->addFile(new SymbolicLink(name, path, time_t_time, link, linkerName, linkPath));
}

// Precondition: program wants to load file system
// Postcondition: file system is loaded
void LoadFile::load(const std::string &path, Shell &shell)
{
	ifstream file(path);

	if (!file.is_open())
		throw runtime_error("File could not be opened.");
	string lineTmp;			// to ignore first line for roots time
	getline(file, lineTmp); // ignore first line
	while (!file.eof())
	{
		string line;
		getline(file, line);
		line = Utils::trim(line);
		if (line.empty())
			continue;
		if (line.substr(line.find(" ") + 1, line.size() - 1) == "Regular")
		{
			addRegularFile(shell, file);
		}
		else if (line.substr(line.find(" ") + 1, line.size() - 1) == "Directory")
		{
			addDirectory(shell, file);
		}
		else if (line.substr(line.find(" ") + 1, line.size() - 1) == "Link")
		{
			addSymbolicLink(shell, file);
		}
		else
		{
			throw runtime_error("Filesystem is not in correct format.");
		}
	}
	Utils::recheckLinks(shell, shell.getRoot()); // recheck links because they may be linked after loading file system
	file.close();
}
