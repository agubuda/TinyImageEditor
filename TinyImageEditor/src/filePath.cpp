#include <iostream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include "filePath.h"

FilePath::FilePath(std::string path)
{
	m_fileName = std::filesystem::path(path).filename().string();
	m_filePath = std::filesystem::path(path).parent_path().string();
	std::replace(m_filePath.begin(), m_filePath.end(), '\\', '/');
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
			//std::cout << "Successfully created directory " << outputPath << std::endl;
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
	int startIndex = filename.size() - 4;

	std::string::size_type Pos = 0;
	Pos = filename.find(".png", startIndex);

	if (Pos != filename.npos)
	{
		return true;
	}
	else
	{
		return false;
	}

	/*
	//you have to determine result with .npos, not with a usigned int.
	if (metaPos != filename.npos)
	{
		return false;
	}
	else
	{
		//std::cout << "Here is a png file. " << std::endl;

		std::string::size_type pngPos = 0;
		pngPos = filename.find(".png");

		//you have to determine result with .npos, not with a usigned int.
		if (pngPos != filename.npos)
		{
			//std::cout << "Here is a png file. " << std::endl;

			return true;

		}
		else
		{
			//std::cout << "No png file," << std::endl;

			return false;

		}
	}
	*/

}

std::vector<std::string> FilePath::ListAllFilenames()
{
	std::vector<std::string> pathRes;
	std::vector<std::string> filenameRes;

	std::string temp;

	if (!std::filesystem::exists(m_filePath))
	{
		std::cout << "plz input valid file path." << std::endl;
		return pathRes;
	}
	for (const auto& file : std::filesystem::recursive_directory_iterator(m_filePath)) {
		//std::cout << file.path() << std::endl;
		{
			temp = file.path().string();
			if (FindExtension(temp)) {
				//res.push_back(file.path().filename().string());
				std::replace(temp.begin(), temp.end(), '\\', '/');
				pathRes.push_back(temp);
				filenameRes.push_back(file.path().filename().string());
				//std::cout << file.path().filename().string() << std::endl;

			}
		}
	}

	return pathRes;

	std::vector<std::string> combineRes;
	std::string::size_type pos = 0;
	std::string src = m_filePath + "/";

	pos = src.find('/', pos);

	while (pos != src.npos)
	{
		std::string temp = src.substr(0, pos);
		//if (combineRes.size() == 1) {
		//	combineRes.push_back("_ConvertResult");
		//}
		combineRes.push_back(temp);
		src = src.substr(pos + 1, src.size());
		pos = src.find('/');
	}

	for (int n = 0; n < pathRes.size(); n++)
	{
		std::stringstream result;

		for (int i = 0; i < combineRes.size(); i++)
		{
			if (i != 0)
			{
				result << "/";
			}
			result << combineRes[i];

			std::filesystem::path outputPath(result.str());
			try
			{
				std::filesystem::create_directory(outputPath);
				//std::cout << "Successfully created directory " << outputPath << std::endl;
			}
			catch (std::filesystem::filesystem_error& e)
			{
				std::cerr << "sth fuuuuuuuuuucking wrong " << e.what() << std::endl;
			}
		}

		result << "/";
		result << pathRes[n];

		pathRes[n] = result.str();
		std::cout << pathRes[n] << std::endl;

	}

	return pathRes;
}
