#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include <ctime>
#include <sstream>

Directory::Directory(const string &name, time_t time, const string &path)
	: File(name, time, path), parentDirectory(nullptr)
{/*Body inintentionally left empty! */}

Directory::Directory(const string &name, time_t time, const string &path, Directory* parentDirectory)
	: File(name, time, path), parentDirectory(parentDirectory)
{/*Body inintentionally left empty! */}

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
		//std::cout << file->getName() << std::endl;
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

Directory* Directory::operator/(const string &name) const
{
	Directory* directory = this->getDirectory(name);
	//if (directory == nullptr) (emin değiliim)
	//	throw runtime_error("cd: " + name + ": No such file or directory");
	return directory;
}
