#pragma once
#include <iostream>

class FilePath
{
public:
	std::string m_filePath;
	std::string m_fileName;

public:
	FilePath(std::string path);
	//~FilePath();


	std::string CombineFilePath();
	static bool FindExtension(std::string filename);
	void ListAllFiles();
};