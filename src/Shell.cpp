#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/SaveFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Executor.hpp"
#include <fstream>
#include <string>

const std::string Shell::fileSystemPath = "./os/filesystem.txt";

Shell::Shell() : isTerminating(false), prompt("cemalBolat:/$ ")
{
	std::ifstream	file(Shell::fileSystemPath);
	time_t			timeT;
	string			timeStr;
	if (!file.is_open())
		throw std::runtime_error("filesystem.txt not found");
	timeStr = Utils::getContent(file);
	timeT = std::stoi(timeStr);
	this->root = new Directory("/", timeT, "/");
	this->currentDirectory = this->root;
	file.close();
}

void Shell::execute(string command)
{
	command = Utils::trim(command);
	string		commandName = command.substr(0, command.find(' '));
	string		commandArgument = command.substr(command.find(' ') + 1, command.size() - 1);

	if (command.find(' ') == string::npos)
		commandArgument = "";
	if (commandName == "exit")
	{
		this->setTerminated(true);
		SaveFile::save("./os/backup.txt", *this);
		return;
	}
	else if (commandName == "ls") // bu kısımlara baK
	{
		if (commandArgument == "-R"){
			Executor::lsRecursive(getCurrentDirectory(), *this);
		}
		else if (commandArgument != ""){
			throw runtime_error("ls: invalid option for cemalBolatShell -- '" + commandArgument + "'");
		}
		else
			Executor::ls(*this);
		return;
	}
	else if (commandName == "cat")
	{
		vector<string>	argsplit = Utils::split(commandArgument, ' ');
		if (argsplit.size() > 1)
			throw runtime_error("cat: invalid number of arguments for cemalBolatShell -- ");
		else
			Executor::cat(*this, commandArgument);
		return;
	}
	else if (commandName == "rm")
	{
		vector<string>	argsplit = Utils::split(commandArgument, ' ');
		if (argsplit.size() > 1)
			throw runtime_error("rm: invalid number of arguments for cemalBolatShell -- ");
		else {
			Executor::rm(*this, commandArgument);
			Utils::recheckLinks(*this, getRoot());
			SaveFile::save(getFileSystemPath(), *this);
			return;
		}
	}
	else if (commandName == "mkdir")
	{
		vector<string> argsplit = Utils::split(commandArgument, ' ');
		if (argsplit.size() > 1)
			throw runtime_error("mkdir: invalid number of arguments for cemalBolatShell -- ");
		else {
			Executor::mkdir(*this, commandArgument);
			Utils::recheckLinks(*this, getRoot());
			SaveFile::save(getFileSystemPath(), *this);
			return;
		}
	}
	else if (commandName == "cd")
	{
		vector<string> argsplit = Utils::split(commandArgument, ' ');
		if (argsplit.size() > 1)
			throw runtime_error("cd: invalid number of arguments for cemalBolatShell -- ");
		else {
			Executor::cd(*this, commandArgument);
			if (getCurrentDirectory()->getName() == "/")
				this->setPrompt("cemalBolat:/$ ");
			else
				setPrompt("cemalBolat:" + getCurrentDirectory()->getPath() + getCurrentDirectory()->getName() + "/$ ");
			return;
		}
	}
	else if (commandName == "cp"){
		vector<string> args = Utils::split(commandArgument, ' ');
		if (args.size() < 2)
			throw runtime_error("cp: missing operand");
		else if (args.size() > 2)
			throw runtime_error("cp: invalid number of arguments for cemalBolatShell -- ");
		Executor::cp(*this, args[0], args[1]);
		Utils::recheckLinks(*this, getRoot());
		SaveFile::save(getFileSystemPath(), *this);
		return;
	}
	else if (commandName == "link"){
		vector<string> args = Utils::split(commandArgument, ' ');
		if (args.size() < 2)
			throw runtime_error("link: missing operand");
		else if (args.size() > 2)
			throw runtime_error("link: invalid number of arguments for cemalBolatShell -- ");
		Executor::link(*this, args[0], args[1]);
		SaveFile::save(getFileSystemPath(), *this);
		return;
	}
	else
		throw runtime_error("cemalBolatShell: " + commandName + ": command not found");

}
