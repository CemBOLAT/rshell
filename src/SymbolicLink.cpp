#include "../includes/SymbolicLink.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"

// link nullptr olabilir !
SymbolicLink::SymbolicLink(const string& name, const string& path, time_t time, File* link, const string& linkedName, const string& linkPath)
	: File(name, time, path), link(link), linkedName(linkedName), linkPath(linkPath)
{/*Body inintentionally left empty! */}

SymbolicLink::~SymbolicLink()
{/*Body inintentionally left empty! */}

void	SymbolicLink::print(ostream& os, size_t maxLen) const
{
	time_t rawtime = this->getTime();
	struct tm* timeinfo = std::localtime(&rawtime);

	Utils::TextEngine::red();
	os << "L ";
	Utils::TextEngine::bold();
	Utils::TextEngine::orange();
	os << std::right << std::setw(maxLen) << std::setfill(' ') << this->getName() << " ";
	Utils::TextEngine::reset();
	Utils::TextEngine::green();
	Utils::printTime(os, timeinfo);
	Utils::TextEngine::cyan();
	os << "2 Bytes ";
	Utils::TextEngine::reset();
	Utils::TextEngine::yellow();
	Utils::TextEngine::bold();
	os << this->getName() << " -> " << this->getLinkedName();
	Utils::TextEngine::reset();

	os << endl;
}

File*	SymbolicLink::getLink() const
{
	return this->link;
}

string	SymbolicLink::getLinkedName() const
{
	return this->linkedName;
}

void	SymbolicLink::setLink(File* link)
{
	this->link = link;
}

string	SymbolicLink::getLinkedPath() const
{
	return this->linkPath;
}

void	SymbolicLink::save(std::ostream &file) const {
	file << "Type: Link" << std::endl;
	file << "Name: " << getName() << std::endl;
	file << "Path: " << getPath() << std::endl;
	file << "Time: " << getTime() << std::endl;
	file << "Link: " << linkPath + "/" + linkedName << std::endl;
}
