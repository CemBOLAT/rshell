#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

void writeTimeToFile(const char *fileName)
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Error opening file for writing." << std::endl;
		return;
	}

	time_t currentTime = time(nullptr);

	std::cout << "Writing time to file: " << 1703968702 << std::endl;

	file << 1703968702;

	file.close();
}

void readTimeFromFile(const char *fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Error opening file for reading." << std::endl;
		return;
	}

	string	storedTime;
	getline(file, storedTime);

	std::struct tm *timeinfo;

	time_t storedTime_t = stoi(storedTime);


	std::cout << "Time read from file: " << storedTime << std::endl;

	file.close();
}

int main()
{
	const char *fileName = "timefile.txt";

	// Write current time to file
	writeTimeToFile(fileName);

	// Read time from file
	readTimeFromFile(fileName);

	return 0;
}
