/*
	Header file for File class
		File is an abstract class
		All files are derived from this class
		Contains pure virtual functions
		Contains static function to find a file in a directory
*/
#ifndef FILE_HPP
# define FILE_HPP

#include <iostream>
#include <string>

using namespace std;

class Shell;

class File {
	public:
		// Constructors and destructor
		File(const string &name, time_t time, const string& path = "");
		File(const File& file) = default;
		File&			operator=(const File& file) = default;
		virtual			~File() = 0;

		// Getters and setters
		time_t			getTime() const { return this->time; }
		string			getName() const { return this->name; }
		string			getPath() const { return this->path; }
		void			setTime(const time_t &time) { this->time = time; }

		// Pure virtual functions
		virtual void	print(ostream& os, size_t maxLen) const = 0;
		virtual void	save(ostream &os) const = 0;
		virtual void	cat() const = 0;
		virtual void	cd(Shell &shell) = 0;

		// static functions to find specific files in a directory
		template <typename T>
		static T		*find(const Shell &shell, const string &name);
	private:
		const string	name;
		string 			path;
		time_t			time;

};


#endif
