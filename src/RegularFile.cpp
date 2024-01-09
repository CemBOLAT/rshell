#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include <ctime>
#include <iomanip>
#include <stdexcept>

RegularFile::RegularFile(const string &name, size_t sizeBytes, time_t time, const string &data, const string &path)
	: File(name, time, path), sizeBytes(sizeBytes), data(data)
{ /*Body inintentionally left empty! */
}

RegularFile::~RegularFile()
{ /*Body inintentionally left empty! */}

// Preconditions: User wants to print the information about this file with using ls or something.
// Postconditions: Prints the information about this file. <colorful way>
void RegularFile::print(ostream &os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm *timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::yellow();
	os << "F ";
	Utils::TextEngine::bold();
	Utils::TextEngine::orange();
	os << std::left << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();
	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::cyan();
	os << this->getSizeBytes() << " Bytes";
	Utils::TextEngine::reset();
	os << endl;
}

// Preconditions : Progaram wants to save this file to filesystem.txt file.
// Postconditions : Saves this file to filesystem.txt file.
void RegularFile::save(std::ostream &file) const
{
	file << "Type: Regular" << std::endl;
	file << "Name: " << getName() << std::endl;
	file << "Path: " << getPath() << std::endl;
	file << "Time: " << getTime() << std::endl;
	file << "Content: " << getData() << std::endl;
}

// Preconditions: User wants to print the content of this file.
// Postconditions: Prints the content of this file.
void RegularFile::cat() const
{
	for (auto it : *this) // iterator (I know it is not a good way to do this but I wanted to use iterator)
	{
		std::cout << it;
	}
	std::cout << std::endl;
}

// Preconditions: User wants to cd this file.
// Postconditions: Throws exception because it is not a directory.
void RegularFile::cd(Shell &shell) // throws exception because it is not a directory
{
	(void)shell; // unused parameter but I need it to override the virtual function
	throw std::runtime_error("cd :" + getName() + " : Not a directory");
}
