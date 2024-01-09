/*
	Header file for RegularFile class.
	RegularFile class is a concrete(derived) class from File class.
			  class has a sizeBytes variable.
			  class has a data string.
			  class has a destructor.
			  class has a iterator subclass. (begin and end functions)
*/
#ifndef REGULARFILE_HPP
# define REGULARFILE_HPP

#include "File.hpp"
#include "Directory.hpp"

class RegularFile : public File {
	public:
		// Constructors and destructor
		RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data = "", const string& path = "");
		RegularFile(const RegularFile& file) = default;
		RegularFile&		operator=(const RegularFile& file) = default;
		virtual				~RegularFile();

		// Getters and setters
		size_t				getSizeBytes() const { return this->sizeBytes; }
		const string		&getData() const { return this->data; }

		// Pure virtual functions
		virtual void		print(ostream& os, size_t maxLen) const override;
		virtual void		save(ostream &os) const override;
		virtual void		cat() const override;
		virtual void		cd(Shell &shell) override;
		// Iterator subclass
		class iterator {
			public:
				iterator(string::const_iterator it) : it(it) {}
				iterator(const iterator& it) = default;
				iterator&	operator=(const iterator& it) = default;
				~iterator() = default;
				char		operator*() { return *it; }
				iterator	&operator++() { ++it; return *this; }
				bool		operator==(const iterator& it) const { return this->it == it.it; }
				bool		operator!=(const iterator& it) const { return this->it != it.it; }
			private:
				string::const_iterator	it;
		};
		iterator	begin() const { return iterator(data.cbegin()); }
		iterator	end() const { return iterator(data.cend()); }
	private:
		size_t			sizeBytes;
		string			data;
};

#endif
