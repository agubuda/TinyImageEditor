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
	std::vector<std::string> ListAllFilePathes();

	static bool FindExtension(std::string filename);
};