#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include <ctime>
#include <iomanip>

RegularFile::RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data, const string& path)
	: File(name, time, path), sizeBytes(sizeBytes), data(data)
{/*Body inintentionally left empty! */}

RegularFile::~RegularFile()
{/*Body inintentionally left empty! */}

ostream&	operator<<(ostream& os, const RegularFile& file)
{
	time_t rawtime = file.getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	os << "F " << file.getName() << " ";
	Utils::printTime(os, timeinfo) << file.getSizeBytes() << " Bytes";
	return os;
}

void		RegularFile::copy(Directory *dir) const
{
	dir->addFile(new RegularFile(*this));
}

void		RegularFile::print(ostream& os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::yellow();
	os << "F ";
	Utils::TextEngine::bold();
	Utils::TextEngine::orange();
	os << std::right << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();
	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::cyan();
	os << this->getSizeBytes() << " Bytes";
	Utils::TextEngine::reset();
	os << endl;
}

void RegularFile::save(std::ostream &file) const
{
	file << "Type: Regular" << std::endl;
	file << "Name: " << getName() << std::endl;
	file << "Path: " << getPath() << std::endl;
	file << "Time: " << getTime() << std::endl;
	file << "Content: " << getData() << std::endl;
}

void RegularFile::cat() const
{
	for (auto c = data.begin(); c != data.end(); ++c)
		std::cout << *c;
	std::cout << std::endl;
}

void RegularFile::cd(Shell& shell)
{
	(void)shell;
	throw std::runtime_error("cd :" + getName() + " : Not a directory");
}
