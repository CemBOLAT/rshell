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

void addRegularFile(Shell &shell, ifstream &file){
	string		name;
	string		path;
	string		content;
	Directory	*directory;
	name = Utils::getContent(file);
	path = Utils::getContent(file);
	content = Utils::getContent(file);
	directory = shell.getRoot()->getDirectory(path);
	if (directory == nullptr)
		throw runtime_error("Directory could not be found."); // not configrated
	directory->addFile(new RegularFile(name, content.size(), time(nullptr), content, path));
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
	}
}
