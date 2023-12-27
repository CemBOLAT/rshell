#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include <ctime>

RegularFile::RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data, const string& path)
	: File(name, time, path), sizeBytes(sizeBytes), data(data)
{/*Body inintentionally left empty! */}
// I can add error check for valid day and month here, but I don't want to.

RegularFile::~RegularFile()
{/*Body inintentionally left empty! */}

ostream&	operator<<(ostream& os, const RegularFile& file)
{
	time_t rawtime = file.getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	os << "F " << file.getName() << " ";
	Utils::printTime(os, timeinfo) << file.getSizeBytes() << " Bytes" << std::endl;
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
