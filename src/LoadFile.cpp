#include "../includes/LoadFile.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include <fstream>
#include <string>

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
	string		data;
	Directory	*directory = shell.getRoot();
	name = Utils::getContent(file);
	path = Utils::getContent(file);
	data = Utils::getData(file);
	directory = Utils::findDirectory(shell.getCurrentDirectory(), path);

	if (directory == nullptr)
		throw runtime_error("Directory coouold not be found."); // not configrated
	directory->addFile(new RegularFile(name, data.size(), time(nullptr), data, path));
}

// hata var iki derinlikte boom
void LoadFile::addDirectory(Shell &shell, ifstream &file){
	string		name;
	string		path;
	Directory	*parentDirectory;
	name = Utils::getContent(file);
	path = Utils::getContent(file);

	if (path == "/"){
		parentDirectory = shell.getRoot();
		parentDirectory->addFile(new Directory(name, time_t(nullptr), path, parentDirectory)); // zaman kritik
	}
	else{
		string		parentPath = Utils::subParentPath(path);
		parentDirectory = Utils::findDirectory(shell.getCurrentDirectory(), parentPath);
		if (parentDirectory == nullptr)
			throw runtime_error("Directory coould not be found."); // not configrated
		parentDirectory->addFile(new Directory(name, time_t(nullptr), path, parentDirectory)); // zaman kritik
	}
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
		//for (size_t i = 0; i < line.size(); i++){
		//	if (line[i] < 32)
		//		printf("%d %ld %ld\n", line[i], i, line.size());
		//}
		//std::cout << line.substr(line.find(" ") + 1, line.size() - 1) << "$" << std::endl;
		if (line.empty())
			continue;
		if (line.substr(line.find(" ") + 1, line.size() - 1) == "Regular"){
			addRegularFile(shell, file);
		}
		else if (line.substr(line.find(" ") + 1, line.size() - 1) == "Directory"){
			addDirectory(shell, file);
		}
	}
	file.close();
}
