/*
	static Savefile class is used to save the current state of the shell to a file.
*/
#ifndef SAVEFILE_HPP
#define SAVEFILE_HPP

class Shell;

#include <iostream>

class SaveFile{
	public:
		SaveFile() = delete;
		SaveFile(const SaveFile& other) = delete;
		SaveFile& operator=(const SaveFile& other) = delete;
		~SaveFile() = delete;

		static void	save(const std::string &path, Shell &shell); 
};

#endif
