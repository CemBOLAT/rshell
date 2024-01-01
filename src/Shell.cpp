#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
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
	timeT = std::stol(timeStr);
	this->root = new Directory("/", timeT, "/");
	this->currentDirectory = this->root;
	file.close();
}

Shell::~Shell()
{
	Utils::terminate(this->root);
}

const string Shell::getFileSystemPath() const
{
	return Shell::fileSystemPath;
}

bool Shell::isTerminated() const
{
	return this->isTerminating;
}

void Shell::setTerminated(bool isTerminated)
{
	this->isTerminating = isTerminated;
}

void Shell::setPrompt(const string &prompt)
{
	this->prompt = prompt;
}

string Shell::getPrompt() const
{
	return this->prompt;
}

Directory *Shell::getRoot() const
{
	return this->root;
}

Directory *Shell::getCurrentDirectory() const
{
	return this->currentDirectory;
}

void Shell::setCurrentDirectory(Directory *directory)
{
	this->currentDirectory = directory;
}

void Shell::execute(string command)
{
	command = Utils::trim(command);
	string commandName = command.substr(0, command.find(' '));
	string commandArgument = command.substr(command.find(' ') + 1, command.size() - 1);

	if (command.find(' ') == string::npos)
		commandArgument = "";
	if (commandName == "exit")
	{
		this->setTerminated(true);
		return;
	}
	else if (commandName == "ls") // bu kısımlara baK
	{
		if (commandArgument == "-R"){
			Executor::lsRecursive(getCurrentDirectory(), *this);
		}
		else
			Executor::ls(*this);
		return;
	}
	else if (commandName == "cat")
	{
		Executor::cat(*this, commandArgument);
		return;
	}
	else if (commandName == "rm")
	{
		Executor::rm(*this, commandArgument);
		Utils::recheckLinks(*this, getRoot());
		return;
	}
	else if (commandName == "mkdir")
	{
		Executor::mkdir(*this, commandArgument);
		Utils::recheckLinks(*this, getRoot());
		return;
	}
	else if (commandName == "cd")
	{
		Executor::cd(*this, commandArgument);
		if (getCurrentDirectory()->getName() == "/")
			this->setPrompt("cemalBolat:/$ ");
		else
			setPrompt("cemalBolat:" + getCurrentDirectory()->getPath() + getCurrentDirectory()->getName() + "/$ ");
		return;
	}
	else if (commandName == "cp"){
		vector<string> args = Utils::split(commandArgument, ' ');
		if (args.size() != 2)
			throw runtime_error("cp: missing operand");
		Executor::cp(*this, args[0], args[1]);
		Utils::recheckLinks(*this, getRoot());
		return;
	}
	else if (commandName == "link"){
		vector<string> args = Utils::split(commandArgument, ' ');
		if (args.size() != 2)
			throw runtime_error("link: missing operand");
		Executor::link(*this, args[0], args[1]);
		return;
	}
	else
		throw runtime_error("cemalBolatShell: " + commandName + ": command not found");

}
