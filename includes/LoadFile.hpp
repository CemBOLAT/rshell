#ifndef LOADFILE_HPP

#define LOADFILE_HPP

class Shell;

#include <iostream>

class LoadFile{
	public:
		LoadFile() = delete;
		LoadFile(const LoadFile& other) = delete;
		LoadFile& operator=(const LoadFile& other) = delete;
		~LoadFile() = delete;

		static void load(const std::string &path, Shell &shell);
};

#endif
