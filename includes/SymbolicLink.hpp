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
		virtual void	print(std::ostream& os, size_t maxLen) const override;

		File*				getLink() const { return this->link; }
		void				setLink(File* link) { this->link = link; }
		string				getLinkedName() const { return this->linkedName; }
		string				getLinkedPath() const { return this->linkPath; }
		virtual void		save(ostream &os) const override;
		//static SymbolicLink *find(const Shell &shell, const string &name, SymbolicLink *ptr);
		virtual void		cat() const override;
	private:
		File*	link;
		string	linkedName;
		string	linkPath;
};

#endif
