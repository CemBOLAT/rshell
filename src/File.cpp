#include "../includes/File.hpp"
#include "../includes/Directory.hpp"
#include "../includes/Shell.hpp"
#include "../includes/Utils.hpp"

File::File(const string &name, time_t time, const string& path)
	: name(name) , path(path) ,time(time)
{/*Body inintentionally left empty! */}


File::~File()
{/*Body inintentionally left empty! */}
