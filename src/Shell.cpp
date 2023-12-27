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

void Shell::execute(string command)
{
	command = Utils::trim(command);
	if (command == "exit")
	{
		this->setTerminated(true);
		return;
	}
	else if (command == "ls")
	{
		Executor::ls(*this);
		return;
	}
	else if (command == "cat"){

	}
}

