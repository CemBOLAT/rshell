#include "../includes/File.hpp"

File::File(const string &name, time_t time, const string& path)
	: name(name) , path(path) ,time(time)
{/*Body inintentionally left empty! */}


File::~File()
{/*Body inintentionally left empty! */}

string File::getName() const
{
	return this->name;
}

string File::setName(const string &name)
{
	this->name = name;
	return this->name;
}

ostream&	operator<<(ostream& os, const File& file)
{
	os << file.getName();
	return os;
}

time_t	File::getTime() const
{
	return this->time;
}

time_t	File::setTime(const time_t &time)
{
	this->time = time;
	return this->time;
}

string	File::getPath() const
{
	return this->path;
}

string	File::setPath(const string &path)
{
	this->path = path;
	return this->path;
}
