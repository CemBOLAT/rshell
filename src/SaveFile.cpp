#include "../includes/SaveFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include <fstream>

namespace {
	void	recursive(std::ofstream &file, Directory *directory){
		for (auto vfile = directory->begin(); vfile != directory->end(); ++vfile){
			(*vfile)->save(file);
			if (dynamic_cast<Directory*>(*vfile) != nullptr){
				Directory *vdFile = dynamic_cast<Directory*>(*vfile);
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
	file << "Time: " << shell.getRoot()->getTime() << std::endl;
	for (const auto &vfile : shell.getRoot()->getFiles()){
		if (vfile != nullptr){
			vfile->save(file);
			if (dynamic_cast<Directory*>(vfile) != nullptr){
				Directory *vdFile = dynamic_cast<Directory*>(vfile);
				recursive(file, vdFile);
			}
		}
	}
	file.close();
}
