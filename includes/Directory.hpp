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
		virtual				~Directory();

		vector<File*>		getFiles() const { return this->files; }
		Directory*			getDirectory(const string &name) const;
		void				addFile(File* file);
		Directory*			getParentDirectory() const { return this->parentDirectory; }
		void				setParentDirectory(Directory* parentDirectory) { this->parentDirectory = parentDirectory; }
		string				getOwnFilesPath() const { return this->ownFilesPath; }
		virtual void		print(ostream& os, size_t maxLen) const override;
		virtual void		save(ostream &os) const override;
		virtual void		cat() const override;
		void				removeFile(const string &name);
	private:
		string			ownFilesPath;
		vector<File*>	files;
		Directory*		parentDirectory;
};


#endif
