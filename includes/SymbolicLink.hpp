/*
	SymbolicLink is another class
		that inherits from File and
		contains a pointer to a File object
		and a string that contains the name of the file
		lastly it contains a string that contains the path of the linked_file
*/
#ifndef SYMBOLIC_LINK_HPP
#define SYMBOLIC_LINK_HPP

#include "File.hpp"
#include <string>

using std::string;

class SymbolicLink : public File
{
	public:
		SymbolicLink(const string& name, const string& path, time_t time, File* link, const string &linkedName, const string &linkPath);
		virtual ~SymbolicLink();
		// Getters and setters
		File*				getLink() const { return this->link; }
		void				setLink(File* link) { this->link = link; }
		string				getLinkedName() const { return this->linkedName; }
		string				getLinkedPath() const { return this->linkPath; }
		// Pure virtual functions
		virtual void		save(ostream &os) const override;
		virtual void		print(std::ostream& os, size_t maxLen) const override;
		virtual void		cat() const override;
		virtual void		cd(Shell &shell) override;
	private:
		File*	link; // pointer to the linked file (Directory, RegularFile or SymbolicLink)
		string	linkedName; // name of the linked file
		string	linkPath; // path of the linked file
};

#endif // SYMBOLIC_LINK_HPP
