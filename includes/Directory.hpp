#ifndef DIRECTORY_HPP
# define DIRECTORY_HPP

#include "File.hpp"
#include <vector>

class Directory : public File {
	public:
		Directory(const string &name, time_t time, const string &path);
		Directory(const string &name, time_t time, const string &path, Directory* parentDirectory);
		Directory(const Directory& dir) = default;
		Directory&	operator=(const Directory& dir) = default;
		virtual		~Directory();


		vector<File*>	getFiles() const;
		friend ostream&	operator<<(ostream& os, const Directory& dir);
		Directory*		getDirectory(const string &name) const;
		void			addFile(File* file);


		template <typename T>
		void			removeFile(const string &name);


		Directory*		getParentDirectory() const;
		void			setParentDirectory(Directory* parentDirectory);

		string			getOwnFilesPath() const;
	private:
		string			ownFilesPath;
		vector<File*>	files;
		Directory*		parentDirectory;
};


#endif
