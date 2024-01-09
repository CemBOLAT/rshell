#include "../includes/SaveFile.hpp"
#include "../includes/SymbolicLink.hpp"
#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include <fstream>
#include <stdexcept>

namespace
{
	// recursive function that saves subdirectories and files
	void recursive(std::ofstream &file, Directory *directory)
	{
		for (auto vfile = directory->getFiles().begin(); vfile != directory->getFiles().end(); ++vfile)
		{
			(*vfile)->save(file);
			if (dynamic_cast<Directory *>(*vfile) != nullptr)
			{
				Directory *vdFile = dynamic_cast<Directory *>(*vfile);
				recursive(file, vdFile);
			}
		}
	}
}

// Precondition: program wants to save the current state of the shell to a file
// Postcondition: the current state of the shell is saved to a file
void SaveFile::save(const std::string &path, Shell &shell)
{
	std::ofstream file(path);
	if (!file.is_open())
	{
		throw std::runtime_error("File cannot be opened!");
	}
	file << "Time: " << shell.getRoot()->getTime() << std::endl;
	for (const auto &vfile : shell.getRoot()->getFiles())
	{
		if (vfile != nullptr)
		{
			vfile->save(file);
			if (dynamic_cast<Directory *>(vfile) != nullptr) // if it is a directory then call recursive function
			{
				Directory *vdFile = dynamic_cast<Directory *>(vfile);
				recursive(file, vdFile); // recursive function
			}
		}
	}
	file.close();
}
