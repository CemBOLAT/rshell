#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include <ctime>

Directory::Directory(const string &name, time_t time, const string &path) : File(name, time, path)
{
	setParentDirectoryName(path);
}

Directory::~Directory()
{/*Body inintentionally left empty! */}

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

void	Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			this->files.erase(it);
			return;
		}
	}
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


ostream&	operator<<(ostream& os, const Directory& dir)
{
	time_t rawtime = dir.getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	os << "D " << dir.getName() << " ";
	Utils::printTime(os, timeinfo);
	return os;
}
