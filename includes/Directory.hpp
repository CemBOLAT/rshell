/*
	Header file for Directory class.
	Directory class is a concrete(derived) class from File class.
			  class has a vector of File pointers.
			  class has a parent directory pointer.
			  class has a own files path string. (path of files in this directory)
			  class has a iterator subclass.
			  class has a destructor deleting all files in files vector.
*/
#ifndef DIRECTORY_HPP
# define DIRECTORY_HPP

#include "File.hpp"
#include <vector>

class Directory : public File {
	public:
		// constructors & destructors
		Directory(const string &name, time_t time, const string &path);
		Directory(const string &name, time_t time, const string &path, Directory* parentDirectory);
		Directory(const Directory& dir) = default;
		Directory&	operator=(const Directory& dir) = default;
		virtual				~Directory();

		// getters & setters
		Directory*			getParentDirectory() const { return this->parentDirectory; }
		string				getOwnFilesPath() const { return this->ownFilesPath; }
		vector<File*>		&getFiles () { return this->files; }
		const vector<File*>	&getFiles () const { return this->files; }

		// virtual functions from File
		virtual void		print(ostream& os, size_t maxLen) const override;
		virtual void		save(ostream &os) const override;
		virtual void		cat() const override;
		virtual void		cd(Shell &shell) override;

		// other member functions
		void				removeFile(const string &name); // removes file from files vector and deletes it <erase>
		void				addFile(File* file); // adds file to files vector <push_back>

		// iterator subclass for Directory
		class iterator {
			public:
				iterator(vector<File *>::const_iterator it) : it(it) {}
				iterator(const iterator& it) = default;
				iterator&	operator=(const iterator& it) = default;
				~iterator() = default;
				File*		operator*() { return *it; }
				iterator	&operator++() { ++it; return *this; }
				bool		operator==(const iterator& it) const { return this->it == it.it; }
				bool		operator!=(const iterator& it) const { return this->it != it.it; }
			private:
				std::vector<File*>::const_iterator		it;
		};

		iterator	begin() const { return iterator(files.cbegin()); }
		iterator	end() const { return iterator(files.cend()); }

	private:
		string			ownFilesPath; // path of files in this directory
		vector<File*>	files; // files in this directory
		Directory*		parentDirectory; // parent directory <roots parent is nullptr>
};


#endif /* DIRECTORY_HPP */
