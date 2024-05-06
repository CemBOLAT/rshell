#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/SaveFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Executor.hpp"
#include <fstream>
#include <string>
#include <stdexcept>

const std::string Shell::fileSystemPath = "./os/filesystem.txt";

Shell::Shell() : isTerminating(false), prompt("/$ ")
{
	std::ifstream file(fileSystemPath);
	time_t timeT;
	string timeStr;
	if (!file.is_open())
		throw std::runtime_error("filesystem.txt not found");
	timeStr = Utils::getContent(file);
	timeT = std::stoi(timeStr);
	this->root = new Directory("/", timeT, "/");
	this->currentDirectory = this->root;
	file.close();
}

/* destructor : deletes root directory \
	deleting root directory deletes all files in it recursively. \
	because delete calls each files destructor and deletes them. */
Shell::~Shell()
{
	delete this->root;
}

void Shell::execute(string command)
{
	if (command == "")
		return;
	command = Utils::trim(command);
	vector<string> commandSplit = Utils::split(command, ' ');
	if (commandSplit.size() == 0)
		return;
	string commandName = commandSplit[0];

	if (commandName == "exit")
	{
		this->setTerminated(true);
		SaveFile::save(Shell::getFileSystemPath(), *this);
		return;
	}
	else if (commandName == "ls")
	{
		if (commandSplit.size() == 1)
			Executor::ls(getCurrentDirectory(), *this, "");
		else if (commandSplit[1] == "-R"){
			Executor::ls(getCurrentDirectory(), *this, commandSplit[1]);
		}
		else
			throw runtime_error("ls: invalid option for cemalBolatShell -- '" + command + "'");
		return;
	}
	else if (commandName == "cat")
	{
		if (commandSplit.size() != 2)
			throw runtime_error("cat: invalid number of arguments for cemalBolatShell -- ");
		else
			Executor::cat(*this, commandSplit[1]);
		return;
	}
	else if (commandName == "rm")
	{
		if (commandSplit.size() != 2)
			throw runtime_error("rm: invalid number of arguments for cemalBolatShell -- ");
		else
		{
			Executor::rm(*this, commandSplit[1]);
			Utils::recheckLinks(*this, getRoot());
			SaveFile::save(getFileSystemPath(), *this);
			return;
		}
	}
	else if (commandName == "mkdir")
	{
		if (commandSplit.size() != 2)
			throw runtime_error("mkdir: invalid number of arguments for cemalBolatShell -- ");
		else
		{
			Executor::mkdir(*this, commandSplit[1]);
			Utils::recheckLinks(*this, getRoot());
			SaveFile::save(getFileSystemPath(), *this);
			return;
		}
	}
	else if (commandName == "cd")
	{
		if (commandSplit.size() > 2)
			throw runtime_error("cd: invalid number of arguments for cemalBolatShell -- ");
		else
		{
			if (commandSplit.size() == 1)
				Executor::cd(*this, "/");
			else
				Executor::cd(*this, commandSplit[1]);
			if (getCurrentDirectory()->getName() == "/")
				this->setPrompt("/$ ");
			else
				setPrompt(getCurrentDirectory()->getPath() + getCurrentDirectory()->getName() + "$ ");
			return;
		}
	}
	else if (commandName == "cp")
	{
		if (commandSplit.size() < 3)
			throw runtime_error("cp: missing operand");
		else if (commandSplit.size() > 3)
			throw runtime_error("cp: invalid number of arguments for cemalBolatShell -- ");
		else if (commandSplit[1] == "." || commandSplit[1] == ".." || commandSplit[1] == "/")
			throw runtime_error("cp: cannot copy to " + commandSplit[1] + ": file exists");
		Executor::cp(*this, commandSplit[1], commandSplit[2]);
		Utils::recheckLinks(*this, getRoot());
		SaveFile::save(getFileSystemPath(), *this);
		return;
	}
	else if (commandName == "link")
	{
		if (commandSplit.size() < 3)
			throw runtime_error("link: missing operand");
		else if (commandSplit.size() > 3)
			throw runtime_error("link: invalid number of arguments for cemalBolatShell -- ");
		Executor::link(*this, commandSplit[1], commandSplit[2]);
		Utils::recheckLinks(*this, getRoot());
		SaveFile::save(getFileSystemPath(), *this);
		return;
	}
	else if (commandName == "clear"){
		Executor::clear();
		return;
	}
	else if (commandName == "touch"){
		Executor::touch(*this, commandSplit);
	}
	else if (commandName == "echo"){
		if (commandSplit.size() < 2)
			throw runtime_error("echo: missing operand");
		else if (commandSplit.size() == 2){
		    std::cout << commandSplit[1] << std::endl;
		    return;
		}
		else if (commandSplit.size() > 3)
			throw runtime_error("echo: invalid number of arguments for cemalBolatShell -- ");
		Executor::echo(*this, commandSplit[1], commandSplit[2]);
		Utils::recheckLinks(*this, getRoot());
		SaveFile::save(getFileSystemPath(), *this);
	}
	
	else
		throw invalid_argument("cemalBolatShell: " + commandName + ": command not found");
}
