#pragma once
#include <iostream>
#include <string>

#include "Main.h"

enum class Formatting
{
	fixedString,
	inputString,
	incrementalNumber,
	date
};


//TODO move definitions to DirectoryNameFormatter.cpp

class NameNode
{
public:
	NameNode()
	{
		//nodeContent = std::make_shared<std::string>(std::string(""));
	}

	~NameNode()
	{

	}

	//std::shared_ptr<std::string> Content()
	const std::string * const Content()
	{
		//return std::make_shared<std::string>(std::string());
		return &nodeContent;
	}

private:
	
	NameNode * nextNode = NULL;
	//NameNode * previousNode = NULL;

	//std::shared_ptr<std::string> nodeContent;
	std::string nodeContent;

};


class DirectoryNameFormatter
{
public:
	DirectoryNameFormatter();
	~DirectoryNameFormatter();

	void CreateNameSequence(); //Here we iterate over our coded name sequence, requesting user input whenever needed, and stuffing each component into a NameNode.
	
	std::string Name(); //traverse the NameNode, adding each content to a string, then return the string.
	
	void SetNameFormat(std::string &newFormat);

private:
	//methods
	std::string NumericalSuffix(unsigned int number, bool padWithZeros = false);//returns a zero-padded, string version of the number, preceeded with an underscore.
	std::string ExtractParentDirectoryPath(const std::string &filePath); //Copied from QnD-ProfCreate
	bool DoesDirectoryExist(const std::string &directory, bool wildCardCheck);
	bool IsDirectoryAccessible(const std::string &path); //Copied from QnD-ProfCreate

	//variables
	NameNode * firstNameNode;
	std::string nameFormat;
};
