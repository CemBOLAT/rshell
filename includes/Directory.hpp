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
		vector<File*>		&getFiles () { return this->files; }
		virtual void		print(ostream& os, size_t maxLen) const override;
		virtual void		save(ostream &os) const override;
		virtual void		cat() const override;
		virtual void		cd(Shell &shell) override;
		void				removeFile(const string &name);

		class Iterator {
			public:
				Iterator(vector<File*>::iterator it) : it(it) {}
				Iterator(const Iterator& it) = default;
				Iterator&	operator=(const Iterator& it) = default;
				~Iterator() = default;
				File*		&operator*() { return *it; }
				Iterator	&operator++() { ++it; return *this; }
				bool		operator==(const Iterator& it) const { return this->it == it.it; }
				bool		operator!=(const Iterator& it) const { return this->it != it.it; }
			private:
				std::vector<File*>::iterator	it;
		};
		Iterator	begin() { return Iterator(files.begin()); }
		Iterator	end() { return Iterator(files.end()); }
	private:
		string			ownFilesPath;
		vector<File*>	files;
		Directory*		parentDirectory;
};


#endif
