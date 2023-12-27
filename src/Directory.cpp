#include "../includes/Directory.hpp"

Directory::Directory(const string &name, time_t time, const string &path) : File(name, time, path)
{
	setParentDirectoryName(path);
}

Directory::~Directory()
{/*Body inintentionally left empty! */}

ostream&	operator<<(ostream& os, const Directory& dir)
{
	os << dir.getName();
	return os;
}

vector<File*>	Directory::getFiles() const
{
	return this->files;
}

// hata dosya okumada sorun var
Directory*	Directory::getDirectory(const string &name) const
{
	if (this->getName() == name){
		return const_cast<Directory*>(this);
	}
	for (auto file : this->files)
	{
		if (file->getName() == name)
		{
			return dynamic_cast<Directory*>(file);
		}
	}
	return nullptr;
}

void	Directory::addFile(File* file)
{
	this->files.push_back(file);
}

// hata olabilir
void	Directory::setParentDirectoryName(const string &name)
{
	if (name == "/")
		this->parentDirectoryName = "";
	else
		this->parentDirectoryName = name.substr(0, name.find_last_of("/"));
}

string	Directory::getParentDirectoryName() const
{
	return this->parentDirectoryName;
}
