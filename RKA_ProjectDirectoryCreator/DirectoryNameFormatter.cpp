#include "DirectoryNameFormatter.h"

DirectoryNameFormatter::DirectoryNameFormatter()
{
}

DirectoryNameFormatter::~DirectoryNameFormatter()
{
}

void DirectoryNameFormatter::SetNameFormat(std::string & newFormat)
{
	nameFormat = newFormat;
}

std::string DirectoryNameFormatter::NumericalSuffix(unsigned int number, bool padWithZeros)
{
	std::string suffix = std::to_string(number);

	if (padWithZeros)
		while (suffix.length() < (floor(log10(MAX_PROJECT_COUNT)) + 1))
		{
			suffix = "0" + suffix;
		}

	return ("_" + suffix);
}

std::string DirectoryNameFormatter::ExtractParentDirectoryPath(const std::string & filePath)
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

bool DirectoryNameFormatter::DoesDirectoryExist(const std::string & directory, bool wildCardCheck)
{
	//std::cout << "Checking existence of directory: " << directory.c_str() << std::endl;
	if (!wildCardCheck) //means we must match entire string
		return std::filesystem::exists(directory);

	//else
	std::string parentDirectory = ExtractParentDirectoryPath(directory);
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

bool DirectoryNameFormatter::IsDirectoryAccessible(const std::string & path)
{
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
