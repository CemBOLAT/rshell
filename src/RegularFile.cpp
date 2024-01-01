#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include "./Template.cpp"
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

size_t		RegularFile::getSizeBytes() const
{
	return this->sizeBytes;
}

string		RegularFile::getData() const
{
	return this->data;
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

RegularFile  *RegularFile::find(const Shell &shell, const std::string &path, RegularFile *ptr)
{
	vector<string> paths = Utils::split(path, '/'); // **
	ptr = findTraverse<RegularFile>(shell.getRoot(), paths);
	return ptr;
}

void RegularFile::cat() const
{
	std::cout << getData() << std::endl;
}
