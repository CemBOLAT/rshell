#include "../includes/UtilsTemplate.hpp"

namespace Utils {
	//sadece bulunduğu dizinde arıyor
	template <typename T>
	T *findFile(const Shell& shell, const string& fileName, const string& currentDirectoryPath) {
		(void)currentDirectoryPath; // unused parameter (for now)
		Directory* currentDirectory = shell.getCurrentDirectory();
		for (auto& file : currentDirectory->getFiles()) {
			if (file->getName() == fileName) {
				if (dynamic_cast<T*>(file) == nullptr)
					throw runtime_error("");
				return dynamic_cast<T*>(file);
			}
		}
		throw invalid_argument("");
		return nullptr;
	}
}
