#ifndef REGULARFILE_HPP
# define REGULARFILE_HPP

#include "File.hpp"
#include "Directory.hpp"

class RegularFile : public File {
	public:
		RegularFile(const string &name, size_t sizeBytes, time_t time, const string& data = "", const string& path = "");
		RegularFile(const RegularFile& file) = default;
		RegularFile&	operator=(const RegularFile& file) = default;
		virtual			~RegularFile();


		size_t			getSizeBytes() const;
		string			getData() const;

		void			copy(Directory *dir) const;
		friend ostream&	operator<<(ostream& os, const RegularFile& file);
		virtual void	print(ostream& os, size_t maxLen) const override;
		virtual void save(ostream &os) const override;
	private:
		size_t			sizeBytes;
		string			data;
};

#endif
