#pragma once
#include <iostream>

class FilePath
{
private:
	std::string m_filePath;
	std::string m_fileName;

public:
	FilePath(std::string path);
	//~FilePath();


	std::string CombineFilePath();
};