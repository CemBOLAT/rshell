#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Executor.hpp"
#include <fstream>
#include <string>

const std::string Shell::fileSystemPath = "./os/filesystem.txt";

Shell::Shell() : isTerminating(false), prompt("cemalBolat:/$ ")
{
	this->root = new Directory("/", time(nullptr), "/");
	this->currentDirectory = this->root;
}

Shell::~Shell()
{
	// free all memory allocated for
	// the file system (dont forget to delete the root directory) too
	delete this->root;
	this->root = nullptr;
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
		return;
	}
	else if (commandName == "mkdir")
	{
		Executor::mkdir(*this, commandArgument);
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
		return;
	}
	else if (commandName == "link"){
		vector<string> args = Utils::split(commandArgument, ' ');
		if (args.size() != 2)
			throw runtime_error("link: missing operand");
		//Executor::link(*this, args[0], args[1]);
		return;
	}

}
