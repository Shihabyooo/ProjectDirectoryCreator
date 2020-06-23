#include "Header.h"

std::string operatingDirectory;
std::string prefix = "\\Project";
std::string clientName = "ClientName";
std::list<std::string> subDirectories;

//TODO replace with forward declaration (or in Header)
std::string NumericalSuffix(unsigned int number, bool padWithZeros) //returns a zero-padded, string version of the number, preceeded with an underscore.
{
	std::string suffix = std::to_string(number);
	
	if (padWithZeros)
		while (suffix.length() < (floor(log10(MAX_PROJECT_COUNT)) + 1))
		{
			suffix = "0" + suffix;
		}

	return ("_" + suffix);
}

bool CheckDirectoryExists(const std::string &directory, bool wildCardCheck)
{
	//std::cout << "Checking existence of directory: " << directory.c_str() << std::endl;
	if (!wildCardCheck) //means we must match entire string
		return std::filesystem::exists(directory);

	//else
	std::string parentDirectory = ExtractParentDirectoryPath(directory);
	//std::cout << "Checking existence of parent directory: " << parentDirectory.c_str() << std::endl;
	if (!std::filesystem::exists(parentDirectory))
	{
		std::cout << "ERROR! Could not resolve parent directory." << std::endl;
		std::cout << "Press Enter to continue." << std::endl;
		std::cin.sync();
		std::cin.get();
		exit(3);
	}

	for (auto& entry : std::filesystem::directory_iterator(parentDirectory))
	{
		//if (entry.is_directory() && IsDirectoryAccessible(entry.path().string()))
		if (entry.is_directory())
		{
			if (directory == entry.path().string().substr(0, directory.length()))
				return true;
		}
	}
	return false;
}

//Copied from QnD-ProfCreate
bool IsDirectoryAccessible(const std::string &path) //used to filter out inaccessabile directories, such as those the OS denies access to.
{
	//std::cout << "Testing accessability for: " << path.c_str() << std::endl;
	try
	{
		std::filesystem::directory_iterator it = std::filesystem::directory_iterator(path);
	}
	catch (std::filesystem::filesystem_error)
	{
		std::cout << "Exception caught when attempting to access directory: " << path.c_str() << ". Access Denied?" << std::endl;
		return false;
	}
	return true;
}

bool IsFileExists(const std::string &path)
{
	return std::filesystem::exists(path);
}

//Copied from QnD-ProfCreate
std::string ExtractParentDirectoryPath(const std::string &filePath)
{
	if (filePath.length() < 1)
		return "";

	size_t counter = 0;
	for (unsigned int i = filePath.length() - 1; i >= 0; i--)
	{
		if (filePath[i] == '\\')
		{
			counter = i;
			break;
		}
	}

	if (counter == 0)
		return "";

	return filePath.substr(0, counter);
}

bool CreateProjectDirectory(const std::string &projectDirectory)
{
	std::cout << "Creating directory: " << projectDirectory.c_str() << std::endl;
	try
	{
		std::filesystem::create_directory(projectDirectory);
	}
	catch (std::filesystem::filesystem_error)
	{
		std::cout << "ERROR! Failed to create directory: " << projectDirectory.c_str() << std::endl;
		std::cout << "Illegal name?" << std::endl;
		return false;
	}

	return true;
}

bool LoadSubdirectories(const std::string &subDirListFile)
{
	if (!IsFileExists(subDirListFile))
	{
		std::cout << "ERROR! Could not find Subdirectories names list in: " << subDirListFile.c_str() << std::endl;
		return false;
	}

	std::ifstream stream;
	stream.open(subDirListFile);
	if (!stream.is_open())
	{
		std::cout << "ERROR! Could not open Subdirectories names list in: " << subDirListFile.c_str() << std::endl;
		return false;
	}

	while (!stream.eof())
	{
		std::string buffer;
		std::getline(stream, buffer);
		if (buffer.length() > 0)
			subDirectories.push_back(std::string(buffer));
	}


	return true;
}

void CreateSubDirectories(const std::string &projectDirectory)
{
	//Load subdirectories list
	if (!LoadSubdirectories(operatingDirectory + "\\Subdirectories.txt"))
		return; //not a program limiting conditions, we can still create the project directory;

	for (std::list<std::string>::iterator it = subDirectories.begin(); it != subDirectories.end(); ++it)
	{
		std::string subDirPath = projectDirectory + "\\" + (*it);
		std::cout << "Creating sub directory: " << subDirPath.c_str() << std::endl;
		std::filesystem::create_directory(subDirPath);
	}

}

int main(int argc, char * argv)
{
	operatingDirectory = std::filesystem::current_path().string();
	
	std::cout << "Enter client name:";
	//std::cin >> std::noskipws >> clientName;
	std::getline(std::cin, clientName);

	unsigned int counter = 1;
	std::string targetDirectory = operatingDirectory + prefix + NumericalSuffix(counter);

	while (counter <= MAX_PROJECT_COUNT)
	{
		targetDirectory = operatingDirectory + prefix + NumericalSuffix(counter);

		if (!CheckDirectoryExists(targetDirectory, true))
			break;

		counter++;

		if (counter > MAX_PROJECT_COUNT)
		{
			std::cout << "ERROR! Number of projects in this folder the allowed Max Project Count." << std::endl;
			std::cout << "Press Enter to continue." << std::endl;
			std::cin.sync();
			std::cin.get();
			return 2;
		}
	}

	targetDirectory += "_" + clientName;
	
	if (!CreateProjectDirectory(targetDirectory))
	{
		std::cout << "Press Enter to continue." << std::endl;
		std::cin.sync();
		std::cin.get();
		return 4;
	}
	
	CreateSubDirectories(targetDirectory);

	std::cout << "Press Enter to continue." << std::endl;
	std::cin.sync();
	std::cin.get();

	return 0;
}