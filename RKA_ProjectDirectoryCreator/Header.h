#pragma once
#include <iostream>
#include <filesystem>
#include <list>
#include <fstream>

#define MAX_PROJECT_COUNT 10000

bool CheckDirectoryExists(const std::string &directory, bool wildCardCheck);
std::string NumericalSuffix(unsigned int number, bool padWithZeros = false); //returns a zero-padded, string version of the number, preceeded with an underscore.
bool IsDirectoryAccessible(const std::string &path); //used to filter out inaccessabile directories, such as those the OS denies access to.
std::string ExtractParentDirectoryPath(const std::string &filePath);
bool IsFileExists(const std::string &path);
bool CreateProjectDirectory(const std::string &projectDirectory);
bool LoadSubdirectories(const std::string &subDirListFile);
void CreateSubDirectories(const std::string &projectDirectory);