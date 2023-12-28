#include "../includes/SaveFile.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include <fstream>

namespace {
	void	recursive(std::ofstream &file, Directory *directory){
		for (const auto &vfile : directory->getFiles()){
			if (dynamic_cast<RegularFile*>(vfile) != nullptr){
				RegularFile *vdFile = dynamic_cast<RegularFile*>(vfile);
				file << "Type: Regular" << std::endl;
				file << "Name: " << vdFile->getName() << std::endl;
				file << "Path: " << vdFile->getPath() << std::endl;
				file << "Content: " << vdFile->getData() << std::endl;
			}
			else if (dynamic_cast<Directory*>(vfile) != nullptr){
				Directory *vdFile = dynamic_cast<Directory*>(vfile);
				file << "Type: Directory" << std::endl;
				file << "Name: " << vdFile->getName() << std::endl;
				file << "Path: " << vdFile->getPath() << std::endl;
				recursive(file, vdFile);
			}
		}
	}
}

void SaveFile::save(const std::string &path, Shell &shell){
	std::ofstream file(path);
	if(!file.is_open()){
		throw std::runtime_error("File cannot be opened!");
	}
	for (const auto &vfile : shell.getRoot()->getFiles()){
		if (dynamic_cast<RegularFile*>(vfile) != nullptr){
			RegularFile *vdFile = dynamic_cast<RegularFile*>(vfile);
			file << "Type: Regular" << std::endl;
			file << "Name: " << vdFile->getName() << std::endl;
			file << "Path: " << vdFile->getPath() << std::endl;
			file << "Content: " << vdFile->getData() << std::endl;
		}
		else if (dynamic_cast<Directory*>(vfile) != nullptr){
			Directory *vdFile = dynamic_cast<Directory*>(vfile);
			file << "Type: Directory" << std::endl;
			file << "Name: " << vdFile->getName() << std::endl;
			file << "Path: " << vdFile->getPath() << std::endl;
			recursive(file, vdFile);
		}

	}
	file.close();
}
