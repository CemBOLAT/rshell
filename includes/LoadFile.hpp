/*
	LoadFile is a static class that loads the file system from a file.
*/
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

		static void		addRegularFile(Shell &shell, ifstream &file); // adds a regular file to the file system
		static void		addDirectory(Shell &shell, ifstream &file); // adds a directory to the file system
		static void		addSymbolicLink(Shell &shell, ifstream &file); // adds a symbolic link to the file system
		static void		load(const std::string &path, Shell &shell); // loads the file system from a file
};

#endif
