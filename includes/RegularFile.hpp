#ifndef REGULARFILE_HPP
# define REGULARFILE_HPP

#include "File.hpp"
#include "Directory.hpp"

class RegularFile : public File {
	public:
		RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data = "", const string& path = "");
		RegularFile(const RegularFile& file) = default;
		RegularFile&		operator=(const RegularFile& file) = default;
		virtual				~RegularFile();


		size_t				getSizeBytes() const { return this->sizeBytes; }
		string				getData() const { return this->data; }

		virtual void		print(ostream& os, size_t maxLen) const override;
		virtual void		save(ostream &os) const override;
		virtual void		cat() const override;
		virtual void		cd(Shell &shell) override;

		class Iterator {
			public:
				Iterator(string::iterator it) : it(it) {}
				Iterator(string::const_iterator it) : cit(it) {}
				Iterator(const Iterator& it) = default;
				Iterator&	operator=(const Iterator& it) = default;
				~Iterator() = default;
				char		&operator*() { return *it; }
				Iterator	&operator++() { ++it; return *this; }
				bool		operator==(const Iterator& it) const { return this->it == it.it; }
				bool		operator!=(const Iterator& it) const { return this->it != it.it; }
			private:
				string::iterator		it;
				string::const_iterator	cit;
		};
		Iterator	begin() { return Iterator(data.begin()); }
		Iterator	end() { return Iterator(data.end()); }
		Iterator	cbegin() const { return Iterator(data.cbegin()); }
		Iterator	cend() const { return Iterator(data.cend()); }
	private:
		size_t			sizeBytes;
		string			data;
};

#endif
