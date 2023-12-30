#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include <ctime>
#include <sstream>

Directory::Directory(const string &name, time_t time, const string &path)
	: File(name, time, path), ownFilesPath(path + name + "/"), parentDirectory(nullptr)
{
	this->files = vector<File*>();
}

Directory::Directory(const string &name, time_t time, const string &path, Directory* parentDirectory)
	: File(name, time, path), ownFilesPath(path + name + "/" ),parentDirectory(parentDirectory)
{
	this->files = vector<File *>();
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
	for (auto file : this->files)
	{
		if (file->getName() == name)
		{
			if (dynamic_cast<Directory*>(file))
				return dynamic_cast<Directory*>(file);
		}
	}

	return nullptr;
}

void	Directory::addFile(File* file)
{
	this->files.push_back(file);
}

string	Directory::getOwnFilesPath() const
{
	return this->ownFilesPath;
}

// hata olabilir
Directory*	Directory::getParentDirectory() const
{
	return this->parentDirectory;
}

void	Directory::setParentDirectory(Directory* parentDirectory)
{
	this->parentDirectory = parentDirectory;
}

ostream&	operator<<(ostream& os, const Directory& dir)
{
	time_t rawtime = dir.getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	os << "D " << dir.getName() << " ";
	Utils::printTime(os, timeinfo);
	return os;
}
