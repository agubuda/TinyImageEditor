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

std::string FilePath::CombineFilePath()
{
	std::vector<std::string> res;
	std::string::size_type pos = 0;
	std::stringstream result;
	std::string src = m_filePath + "/";

	pos = src.find('/', pos);

	while (pos != src.npos)
	{
		std::string temp = src.substr(0, pos);
		if (res.size() == 1) {
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

		std::filesystem::path outputPath(result.str());
		try
		{
			std::filesystem::create_directory(outputPath);
			std::cout << "Successfully created directory " << outputPath << std::endl;
		}
		catch (std::filesystem::filesystem_error& e)
		{
			std::cerr << "sth fuuuuuuuuuucking wrong " << e.what() << std::endl;
		}
	}


	result << "/";
	result << m_fileName;

	return result.str();
};

bool FilePath::FindExtension()
{
	std::string::size_type pos = 0;
	if (pos != m_fileName.npos)
	{
		m_fileName.find(".png");
	}

	return true;
}