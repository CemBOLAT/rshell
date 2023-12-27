#ifndef DIRECTORY_HPP
# define DIRECTORY_HPP

#include "File.hpp"
#include <vector>

class Directory : public File {
	public:
		Directory(const string &name, time_t time, const string &str);
		Directory(const Directory& dir) = default;
		Directory&	operator=(const Directory& dir) = default;
		virtual		~Directory();


		vector<File*>	getFiles() const;
		friend ostream&	operator<<(ostream& os, const Directory& dir);
		Directory*		getDirectory(const string &name) const;
		void			setParentDirectoryName(const string &name);
		string			getParentDirectoryName() const;
		void			addFile(File* file);
		void			removeFile(const string &name);
	private:
		vector<File*>	files;
		string			parentDirectoryName;
};

#endif
