#include "../includes/SaveFile.hpp"
#include "../includes/SymbolicLink.hpp"
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
				file << "Time: " << vdFile->getTime() << std::endl;
				file << "Content: " << vdFile->getData() << std::endl;
			}
			else if (dynamic_cast<Directory*>(vfile) != nullptr){
				Directory *vdFile = dynamic_cast<Directory*>(vfile);
				file << "Type: Directory" << std::endl;
				file << "Name: " << vdFile->getName() << std::endl;
				file << "Path: " << vdFile->getPath() << std::endl;
				file << "Time: " << vdFile->getTime() << std::endl;
				recursive(file, vdFile);
			}
			else if (dynamic_cast<SymbolicLink*>(vfile) != nullptr){
				SymbolicLink *vdFile = dynamic_cast<SymbolicLink*>(vfile);
				file << "Type: Link" << std::endl;
				file << "Name: " << vdFile->getName() << std::endl;
				file << "Path: " << vdFile->getPath() << std::endl;
				file << "Time: " << vdFile->getTime() << std::endl;
				if (vdFile->getLink()->getPath() == "/")
					file << "Link: " << "/" + vdFile->getLink()->getName() << std::endl;
				else
					file << "Link: " << vdFile->getLink()->getPath() + "/" + vdFile->getLink()->getName() << std::endl;
			}
		}
	}
}


// poly kullan sonra bAK
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
			file << "Time: " << vdFile->getTime() << std::endl;
			file << "Content: " << vdFile->getData() << std::endl;
		}
		else if (dynamic_cast<Directory*>(vfile) != nullptr){
			Directory *vdFile = dynamic_cast<Directory*>(vfile);
			file << "Type: Directory" << std::endl;
			file << "Name: " << vdFile->getName() << std::endl;
			file << "Path: " << vdFile->getPath() << std::endl;
			file << "Time: " << vdFile->getTime() << std::endl;
			recursive(file, vdFile);
		}
		else if (dynamic_cast<SymbolicLink*>(vfile) != nullptr){
			SymbolicLink *vdFile = dynamic_cast<SymbolicLink*>(vfile);
			file << "Type: Link" << std::endl;
			file << "Name: " << vdFile->getName() << std::endl;
			file << "Path: " << vdFile->getPath() << std::endl;
			file << "Time: " << vdFile->getTime() << std::endl;
			if (vdFile->getLink()->getPath() == "/")
				file << "Link: "
					 << "/" + vdFile->getLink()->getName() << std::endl;
			else
				file << "Link: " << vdFile->getLink()->getPath() + "/" + vdFile->getLink()->getName() << std::endl;
		}
	}
	file.close();
}
