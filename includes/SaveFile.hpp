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
