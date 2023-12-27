#include "../includes/RegularFile.hpp"
#include <iomanip>
#include <ctime>

RegularFile::RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data, const string& path)
	: File(name, time, path), sizeBytes(sizeBytes), data(data)
{/*Body inintentionally left empty! */}
// I can add error check for valid day and month here, but I don't want to.

RegularFile::~RegularFile()
{/*Body inintentionally left empty! */}

ostream&	operator<<(ostream& os, const RegularFile& file)
{
	// ayrı bölersin bu kısmı!

	time_t rawtime = file.getTime();

	struct tm* timeinfo = std::localtime(&rawtime);
	int day = timeinfo->tm_mday;
	int month = timeinfo->tm_mon + 1;
	int hour = timeinfo->tm_hour;
	int minute = timeinfo->tm_min;

	os << "F " << file.getName() << " "
		<< std::setw(2) << std::setfill('0') << day << "/"
		<< std::setw(2) << std::setfill('0') << month << " "
		<< std::setw(2) << std::setfill('0') << hour << ":"
		<< std::setw(2) << std::setfill('0') << minute << " "
		<< file.getSizeBytes() << " Bytes" << std::endl;
	return os;
}

size_t		RegularFile::getSizeBytes() const
{
	return this->sizeBytes;
}
