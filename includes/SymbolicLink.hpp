#ifndef SYMBOLIC_LINK_HPP
#define SYMBOLIC_LINK_HPP

#include <string>
#include "File.hpp"
#include "RegularFile.hpp"

class SymbolicLink : public File
{
	public:
		SymbolicLink(const string& name, const string& path, time_t time, File* link);
		virtual ~SymbolicLink();
		virtual void	print(std::ostream& os, size_t maxLen) const override;

		File*			getLink() const;
		void			setLink(File* link);
		virtual void save(ostream &os) const override;
	private:
		File* link;
};

#endif
