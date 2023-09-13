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

bool FilePath::FindExtension(std::string filename)
{
	std::string::size_type pos = 0;
	pos = filename.find(".png");

	//you have to determine result with .npos, not with a usigned int.
	if (pos != filename.npos)
	{
		std::cout << "Here is a png file. " << std::endl;

		return true;

	}
	else
	{
		std::cout << "No png file," << std::endl;

		return false;

	}
}

std::vector<std::string> FilePath::ListAllFiles()
{
	std::vector<std::string> res;
	if (!std::filesystem::exists(m_filePath))
	{
		std::cout << "plz input valid file path." << std::endl;
		return;
	}
	for (const auto& file : std::filesystem::directory_iterator(m_filePath)) {
		std::cout << file.path() << std::endl;
		{
			std::cout << file.path().filename().string() << std::endl;
			if(FindExtension(file.path().filename().string()))
			res.push_back(file.path().filename().string());

		}
	}

	return res;

}
