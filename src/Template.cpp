#include "../includes/Directory.hpp"

template <typename T>
void Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if ((*it)->getName() == name && dynamic_cast<T *>(*it))
		{
			this->files.erase(it);
			delete *it;
			*it = nullptr;
			return;
		}
	}
}

template <typename T>
T	*findTraverse(Directory *directory, const vector<string> &path)
{
	for (auto file : directory->getFiles())
	{
		if (file->getName() == path[0])
		{
			if (path.size() == 1)
			{
				if (dynamic_cast<T *>(file))
					return dynamic_cast<T *>(file);
			}
			else
			{
				return findTraverse<T>(dynamic_cast<Directory *>(file), vector<string>(path.begin() + 1, path.end()));
			}
		} // throw olabilir
	}
	return nullptr;
}
