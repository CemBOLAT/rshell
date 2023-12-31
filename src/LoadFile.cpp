#include "../includes/LoadFile.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

#include "../includes/Utils.hpp"

using namespace std;

// Dosyalar için:
	// 1. Dosya Türü (Regular ise)
		// 2. Dosya Adı
		// 3. Dosya Konumu
		// 4. Dosya İçeriği
	// 2. Dosya Türü (Directory ise)
		// 2. Dosya Adı
		// 3. Dosya Konumu

void LoadFile::addRegularFile(Shell &shell, ifstream &file){
	string		name;
	string		path;
	string		time;
	string		data;
	Directory	*directory = shell.getRoot();
	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);
	data = Utils::getData(file);

	time_t time_t_time = stoi(time);

	// string to time_t conversion
	// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c

	if (path == "/"){
		directory = shell.getRoot();
		directory->addFile(new RegularFile(name, data.size(), time_t_time, data, path));
		return;
	}
	directory = Directory::find(shell, path, nullptr);

	if (directory == nullptr)
		throw runtime_error("Directory coouold not be found."); // not configrated
	directory->addFile(new RegularFile(name, data.size(), time_t_time, data, path));
}

// hata var iki derinlikte boom
void LoadFile::addDirectory(Shell &shell, ifstream &file){
	string		name;
	string		path;
	string		time;
	Directory	*parentDirectory;

	//Directory	*myDirectory;
	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);

	time_t time_t_time = stoi(time);


	if (path == "/"){
		parentDirectory = shell.getRoot();
		parentDirectory->addFile(new Directory(name, time_t_time, path, parentDirectory)); // zaman kritik
	}
	else{
		parentDirectory = Directory::find(shell, path, nullptr);
		if (parentDirectory == nullptr)
			throw runtime_error("Directory coould not be found."); // not configrated
		//myDirectory = parentDirectory / name;
		parentDirectory->addFile(new Directory(name, time_t_time, path, parentDirectory)); // zaman kritik
	}
}

// çöküyor !!
void	LoadFile::addSymbolicLink(Shell &shell, ifstream &file){
	string	name;
	string	path;
	string	time;
	string	linkPath;
	string	linkerName;

	name = Utils::getContent(file);
	path = Utils::getContent(file);
	time = Utils::getContent(file);
	linkPath = Utils::getContent(file);
	linkerName = linkPath.substr(linkPath.find_last_of("/") + 1, linkPath.size() - 1);

	time_t time_t_time = stoi(time);

	RegularFile *link;
	link = RegularFile::find(shell, linkPath, link);
	Directory *linkDirectory;
	linkDirectory = Directory::find(shell, linkPath, linkDirectory);
	Directory *directory;
	directory = Directory::find(shell, path, directory);

	if (link == nullptr && linkDirectory == nullptr){
		directory->addFile(new SymbolicLink(name, path, time_t_time, nullptr, linkerName, linkPath));
	}
	else if (link != nullptr)
		directory->addFile(new SymbolicLink(name, path, time_t_time, link, linkerName, linkPath));
	else if (linkDirectory != nullptr)
		directory->addFile(new SymbolicLink(name, path, time_t_time, linkDirectory, linkerName, linkPath));
	else
		throw runtime_error("Link could not be added.");
}

void LoadFile::load(const std::string &path, Shell &shell){
	ifstream file(path);

	if (!file.is_open())
		throw runtime_error("File could not be opened.");
	while (!file.eof())
	{
		string line;
		getline(file, line);
		line = Utils::trim(line);
		if (line.empty())
			continue;
		if (line.substr(line.find(" ") + 1, line.size() - 1) == "Regular"){
			addRegularFile(shell, file);
		}
		else if (line.substr(line.find(" ") + 1, line.size() - 1) == "Directory"){
			addDirectory(shell, file);
		}
		else if (line.substr(line.find(" ") + 1, line.size() - 1) == "Link"){
			addSymbolicLink(shell, file);
		}
		else{
			throw runtime_error("Filesystem is not in correct format.");
		}
	}
	file.close();
}
