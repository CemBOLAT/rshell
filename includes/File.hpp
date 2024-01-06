#ifndef FILE_HPP
# define FILE_HPP

#include <iostream>
#include <string>

using namespace std;

class Shell;

// load ve save kısmını da pure virtual yap
class File {
	public:
		File(const string &name, time_t time, const string& path = "");
		File(const File& file) = default;
		File&			operator=(const File& file) = default;
		virtual			~File() = 0;

		time_t			getTime() const { return this->time; }
		void			setTime(const time_t &time) { this->time = time; }

		string			getName() const { return this->name; }
		string			getPath() const { return this->path; }

		virtual void	print(ostream& os, size_t maxLen) const = 0;
		virtual void	save(ostream &os) const = 0;
		virtual void	cat() const = 0;
		virtual void	cd(Shell &shell) = 0;

		template <typename T>
		static T		*find(const Shell &shell, const string &name);
	private:
		const string	name;
		string 			path;
		time_t			time;

};


#endif
