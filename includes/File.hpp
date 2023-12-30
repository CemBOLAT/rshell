#ifndef FILE_HPP
# define FILE_HPP

#include <iostream>
#include <string>

using namespace std;

class File {
	public:
		File(const string &name, time_t time, const string& path = "");
		File(const File& file) = default;
		File&			operator=(const File& file) = default;
		virtual			~File() = 0;

		time_t			getTime() const;
		time_t			setTime(const time_t &time);

		string			getName() const;
		string			setName(const string &name);
		string			getPath() const;
		string			setPath(const string &path);

		friend ostream&	operator<<(ostream& os, const File& file);
		virtual void	print(ostream& os, size_t maxLen) const = 0;
	private:
		string			name;
		string 			path;
		time_t			time;

};

#endif
