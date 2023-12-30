#include "../includes/Directory.hpp"

template <typename T>
void Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if ((*it)->getName() == name && dynamic_cast<T *>(*it))
		{
			this->files.erase(it);
			return;
		}
	}
}
