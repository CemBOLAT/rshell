#include "../includes/SymbolicLink.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include <ctime>
#include <iomanip>
#include <stdexcept>

SymbolicLink::SymbolicLink(const string &name, const string &path, time_t time, File *link, const string &linkedName, const string &linkPath)
	: File(name, time, path), link(link), linkedName(linkedName), linkPath(linkPath)
{ /*Body inintentionally left empty! */
}

SymbolicLink::~SymbolicLink()
{ /*Body inintentionally left empty! */}

// Preconditions: User wants to print the information about this file with using ls or something.
// Postconditions: Prints the information about this file. <colorful way>
void SymbolicLink::print(ostream &os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm *timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::red();
	os << "L ";
	Utils::TextEngine::bold();
	Utils::TextEngine::orange();
	os << std::left << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();
	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::cyan();
	os << "2 Bytes ";
	Utils::TextEngine::reset();
	Utils::TextEngine::yellow();
	Utils::TextEngine::bold();
	os << this->getName() << " -> " << this->getLinkedPath() + this->getLinkedName();
	Utils::TextEngine::reset();

	os << endl;
}

// Preconditions : Progaram wants to save this file to filesystem.txt file.
// Postconditions : Saves this file to filesystem.txt file.
void SymbolicLink::save(std::ostream &file) const
{
	file << "Type: Link" << std::endl;
	file << "Name: " << getName() << std::endl;
	file << "Path: " << getPath() << std::endl;
	file << "Time: " << getTime() << std::endl;
	file << "Link: " << linkPath << std::endl;
	file << "LinkedName: " << linkedName << std::endl;
}

// Preconditions: User wants to print the content of this file.
// Postconditions: Prints the content of this file.
void SymbolicLink::cat() const
{
	if (link == nullptr)
	{
		throw std::runtime_error("cat :" + getName() + " : No such file or directory");
	}
	link->cat(); // if link is symbolic link, it will call this function again
}


// Preconditions: User wants to cd this file.
// Postconditions: Throws exception because it is not a directory.
void SymbolicLink::cd(Shell &shell)
{
	if (link == nullptr)
	{
		throw std::runtime_error("cd :" + getName() + " : No such file or directory");
	}
	link->cd(shell); // if link is symbolic link, it will call this function again
}
