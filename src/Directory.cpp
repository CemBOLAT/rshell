#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/Shell.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

Directory::Directory(const string &name, time_t time, const string &path)
	: File(name, time, path), ownFilesPath("/"), parentDirectory(nullptr)
{
	this->files = vector<File *>();
}

Directory::Directory(const string &name, time_t time, const string &path, Directory *parentDirectory)
	: File(name, time, path), ownFilesPath(path + name + "/"), parentDirectory(parentDirectory)
{
	this->files = vector<File *>();
}

Directory::~Directory()
{ /*Body inintentionally left empty! */
}

// hata dosya okumada sorun var
Directory *Directory::getDirectory(const string &name) const
{
	for (auto file : this->files)
	{
		if (file->getName() == name)
		{
			if (dynamic_cast<Directory *>(file))
				return dynamic_cast<Directory *>(file);
		}
	}

	return nullptr;
}

void Directory::addFile(File *file)
{
	this->files.push_back(file);
}

void Directory::print(std::ostream &os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm *timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::yellow();
	os << "D ";
	Utils::TextEngine::reset();
	Utils::TextEngine::bold();
	Utils::TextEngine::cyan();
	os << std::right << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();

	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::resetAll();
	os << std::endl;
}

void Directory::save(std::ostream &file) const
{
	file << "Type: Directory" << std::endl;
	file << "Name: " << this->getName() << std::endl;
	file << "Path: " << this->getPath() << std::endl;
	file << "Time: " << this->getTime() << std::endl;
}

void Directory::cat() const
{
	throw std::runtime_error("cat: " + this->getName() + ": Is a directory");
}

void Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			if (dynamic_cast<Directory *>(*it))
				Utils::recRemoveDir(dynamic_cast<Directory *>(*it));
			this->files.erase(it);
			return;
		}
	}
}

void Directory::cd(Shell &shell)
{
	shell.setCurrentDirectory(this);
}
