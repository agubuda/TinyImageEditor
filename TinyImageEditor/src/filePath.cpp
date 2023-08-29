#include <iostream>
#include <filesystem>
#include <sstream>
#include "filePath.h"

FilePath::FilePath(std::string path)
{
	m_fileName = std::filesystem::path(path).filename().string();
	m_filePath = std::filesystem::path(path).parent_path().string();
	//m_filePath = path;
}

//std::string FilePath::GetPath(std::string path)
//{
//};
//
//std::string GetFileName(std::string path)
//{
//	//FilePath::m_filePaht = std::filesystem::path(path).filename().string();
//};

std::string FilePath::CombineFilePath()
{
	std::vector<std::string> res;
	std::string::size_type pos = 0;
	std::stringstream result;
	std::string src = m_filePath + "/" + m_fileName + "/";

	pos = src.find('/', pos);

	while (pos != src.npos)
	{
		std::string temp = src.substr(0, pos);
		if (res.size() == 2) {
			res.push_back("_ConvertResult");
		}
		res.push_back(temp);
		src = src.substr(pos + 1, src.size());
		pos = src.find('/');
	}

	for (int i = 0; i<res.size(); i++)
	{
		if (i != 0)
		{
			result << "/";
		}
		result << res[i];
		std::cout << result.str() << std::endl;
	}

	return result.str();
};