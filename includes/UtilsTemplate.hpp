#ifndef UTILSTEMPLATE_HPP

# define UTILSTEMPLATE_HPP

# include <string>
# include <fstream>
# include "Shell.hpp"

class Shell;

using namespace std;

namespace Utils {
	template <typename T = Directory>
	// muhtemelen bozuk !cl
	T *findFile(const Shell& shell, const string& fileName) {
		// fileName might be a path like /home/ali/Desktop/file.txt
		// or just a file name like file.txt
		// or a dependent path like ../file.txt
		// or a dependent path like ../../file.txt(bla bla bla)

		Directory *currentDirectory = shell.getCurrentDirectory();
		vector<string> path = split(fileName, '/');
		if (path.size() == 1) {
			// fileName is just a file name
			for (auto& file : currentDirectory->getFiles()) {
				if (file->getName() == fileName) {
					if (dynamic_cast<T*>(file) == nullptr)
						throw runtime_error("");
					return dynamic_cast<T*>(file);
				}
			}
		}
		else {
			for (size_t i = 0; i < path.size() - 2; i++) {
				if (path[i] == ".") {
					// do nothing
				}
				else if (path[i] == "..") {
					currentDirectory = currentDirectory->getParentDirectory();
				}
				else {
					// path[i] is a directory name
					currentDirectory = currentDirectory->getDirectory(path[i]);
				}
			}

		}

		for (auto &file : currentDirectory->getFiles())
		{
			if (file->getName() == path.back())
			{
				if (dynamic_cast<T *>(file) == nullptr)
				{
					throw runtime_error("Invalid file type");
				}
				return dynamic_cast<T *>(file);
			}
		}

		// File not found in the specified path
		throw invalid_argument("File not found: " + fileName);
		return nullptr;
	}
}



#endif
