#ifndef LOADFILE_HPP

#define LOADFILE_HPP

class Shell;

#include <iostream>
#include <fstream>

using namespace std;

class LoadFile{
	public:
		LoadFile() = delete;
		LoadFile(const LoadFile& other) = delete;
		LoadFile& operator=(const LoadFile& other) = delete;
		~LoadFile() = delete;

		static void		load(const std::string &path, Shell &shell);
		static void		addRegularFile(Shell &shell, ifstream &file);
		static void		addDirectory(Shell &shell, ifstream &file);
};

#endif
