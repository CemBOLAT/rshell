#include "../includes/Shell.hpp"
#include "../includes/RegularFile.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Executor.hpp"
#include <fstream>
#include <string>

const std::string Shell::fileSystemPath = "./os/filesystem.txt";

Shell::Shell() :
		isTerminating(false), prompt("cemalBolat:/$ "), currentDirectoryPath("/")
{
	this->root = new Directory("/", time(nullptr), "/");
	this->currentDirectory = this->root;
}

Shell::~Shell()
{
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

void Shell::setPrompt(const string& prompt)
{
	this->prompt = prompt;
}

string Shell::getPrompt() const
{
	return this->prompt;
}

void Shell::setCurrentDirectoryPath(const string& path)
{
	this->currentDirectoryPath = path;
}

string Shell::getCurrentDirectoryPath() const
{
	return this->currentDirectoryPath;
}

Directory* Shell::getRoot() const
{
	return this->root;
}

Directory* Shell::getCurrentDirectory() const
{
	return this->currentDirectory;
}

Directory* Shell::setCurrentDirectory(Directory* directory)
{
	this->currentDirectory = directory;
	return this->currentDirectory;
}

void Shell::execute(string command)
{
	command = Utils::trim(command);
	string commandName = command.substr(0, command.find(' '));
	string commandArgument = command.substr(command.find(' ') + 1, command.size() - 1);
	if (commandName == "exit")
	{
		this->setTerminated(true);
		return;
	}
	else if (command == "ls") // bu kısımlara baK
	{
		Executor::ls(*this);
		return;
	}
	else if (commandName == "cat"){
		Executor::cat(*this, commandArgument);
		return;
	}
	else if (commandName == "rm"){
		Executor::rm(*this, commandArgument);
		return;
	}
}

