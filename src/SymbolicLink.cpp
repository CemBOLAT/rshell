#include "../includes/SymbolicLink.hpp"
#include "../includes/Utils.hpp"
#include "../includes/TextEngine.hpp"
#include <ctime>
#include <iomanip>

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
	os << this->getName() << " -> " << this->getLinkedPath() + this->getLinkedName();
	Utils::TextEngine::reset();

	os << endl;
}

void	SymbolicLink::save(std::ostream &file) const {
	file << "Type: Link" << std::endl;
	file << "Name: " << getName() << std::endl;
	file << "Path: " << getPath() << std::endl;
	file << "Time: " << getTime() << std::endl;
	file << "Link: " << linkPath << std::endl;
	file << "LinkedName: " << linkedName << std::endl;
}

void SymbolicLink::cat() const
{
	if (link == nullptr)
	{
		throw std::runtime_error("cat :" + getName() + " : No such file or directory");
	}
	link->cat();
}

void SymbolicLink::cd(Shell& shell)
{
	if (link == nullptr)
	{
		throw std::runtime_error("cd :" + getName() + " : No such file or directory");
	}
	link->cd(shell);
}
