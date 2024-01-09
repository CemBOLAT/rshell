#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "../includes/Shell.hpp"
#include <ctime>		// std::localtime
#include <sstream>		// std::stringstream
#include <iomanip>		// std::setw
#include <stdexcept>	// std::runtime_error

// constructor for root directory
Directory::Directory(const string &name, time_t time, const string &path)
	: File(name, time, path), ownFilesPath("/"), parentDirectory(nullptr)
{
	files = vector<File *>();
}

// constructor for other directories
Directory::Directory(const string &name, time_t time, const string &path, Directory *parentDirectory)
	: File(name, time, path), ownFilesPath(path + name + "/"), parentDirectory(parentDirectory)
{
	files = vector<File *>();
}

// destructor : delete calls each files destructor and deletes them.
// then deletes itself.
Directory::~Directory()
{
	for (auto file : this->files)
		delete file;
}

// Preconditions: User wants to add a file to this directory.
// Postconditions: Adds file to files vector. <push_back>
void Directory::addFile(File *file)
{
	this->files.push_back(file);
}

// Preconditions: User wants to print the information about this directory with using ls or something.
// Postconditions: Prints the information about this directory. <colorful way>
void Directory::print(std::ostream &os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm *timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::yellow();
	os << "D ";
	Utils::TextEngine::reset();
	Utils::TextEngine::bold();
	Utils::TextEngine::cyan();
	os << std::left << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();

	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::resetAll();
	os << std::endl;
}

// Preconditions : Progaram wants to save this directory to filesystem.txt file.
// Postconditions : Saves this directory to filesystem.txt file.
void Directory::save(std::ostream &file) const
{
	file << "Type: Directory" << std::endl;
	file << "Name: " << this->getName() << std::endl;
	file << "Path: " << this->getPath() << std::endl;
	file << "Time: " << this->getTime() << std::endl;
}

// Preconditions: User wants to cat this directory.
// Postconditions: Throws runtime_error.
void Directory::cat() const
{
	throw std::runtime_error("cat: " + this->getName() + ": Is a directory");
}

// Preconditions: User wants to remove a specific named file from this directory.
// Postconditions: Removes file from files vector and deletes it <erase>
void Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			delete *it;
			this->files.erase(it);
			return;
		}
	}
}

// Preconditions: User wants to change current directory to this directory.
// Postconditions: Changes current directory to this directory.
void Directory::cd(Shell &shell)
{
	shell.setCurrentDirectory(this);
}
