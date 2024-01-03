#include "../includes/Directory.hpp"
#include "../includes/Utils.hpp"
#include <memory>

template <typename T>
void Directory::removeFile(const string &name)
{
	for (auto it = this->files.begin(); it != this->files.end(); ++it)
	{
		if (((*it)->getName() == name && dynamic_cast<T *>(*it)))
		{
			delete *it;
			this->files.erase(it);
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
		}
	}
	return nullptr;
}

template <>
Directory	*File::find<Directory>(const Shell &shell, const string &path)
{
	vector<string> paths = Utils::split(path, '/'); // **
	if (paths.size() == 0)
	{
		return shell.getRoot();
	}
	return (findTraverse<Directory>(shell.getRoot(), paths));
}

template <typename T>
T	*File::find(const Shell &shell, const string &path)
{
	vector<string> paths = Utils::split(path, '/'); // **
	return (findTraverse<T>(shell.getRoot(), paths));
}
