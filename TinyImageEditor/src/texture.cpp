#include "GLFW/glfw3.h"
#include "opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <filesystem>

//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include "texture.h"
#include "windows.h"
#include "filePath.h"

//using namespace std;
//using namespace cv;

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize.h"

Texture::Texture(const std::string& path)
	:image_id(NULL), m_textureID(0), path(NULL), fileName(NULL),
	m_LocalBuffer(nullptr), width(0), height(0), nrChannels(0)
{
	cv::Mat m_texture;
	m_texture = cv::imread(path.c_str(), cv::IMREAD_ANYCOLOR);
	cv::cvtColor(m_texture, m_texture, cv::COLOR_RGB2BGR);

	//m_LocalBuffer = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture.cols, m_texture.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, m_texture.data);

	image_id = (GLuint*)m_textureID;
	//glGenerateMipmap();

	//if (m_LocalBuffer) {
	//    stbi_image_free(m_LocalBuffer);
	//}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

//unsigned char* Texture::LoadSingleImage(const std::string& inPath) {
//    unsigned char* data;
//    int width, height, nrChannels;
//    data = stbi_load(inPath.c_str(), &width, &height, &nrChannels, 0);
//    return data;
//}

void Texture::ResizeImage(const std::string& inPath, const std::string& outPath, int outputChannel[])
{
	//int width, height, nrChannels;
	//unsigned char* data = NULL;
	//data = stbi_load(inPath.c_str(), &width, &height, &nrChannels, 3);

	//unsigned int outputDataMem = outputChannel[0] *  outputChannel[1] * nrChannels;
	//unsigned char* outData = (unsigned char*)malloc(outputDataMem);

	////stbir_resize(data, width, height, nrChannels, outData, outputChannel[0], outputChannel[1], outputChannel[2], STBIR_TYPE_UINT8, nrChannels, STBIR_ALPHA_CHANNEL_NONE, 0,
	////    STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
	////    STBIR_FILTER_BOX, STBIR_FILTER_BOX,
	////    STBIR_COLORSPACE_SRGB, nullptr
	////            );

	//stbir_resize_uint8(data, width, height, 0,
	//    outData, outputChannel[0], outputChannel[1], 0, nrChannels);

	////std::filesystem::create_directory(outPath);
	////std::filesystem::
	//if ((outputChannel[2] > 0) && (outputChannel[2] < 5)) {
	//    stbi_write_png(outPath.c_str(), outputChannel[0], outputChannel[1], nrChannels, outData, 0);
	//    std::cout << "input channel Out of range." << std::endl;

	//}

	//stbi_image_free(data);
	//stbi_image_free(outData);

	std::cout << outputChannel << std::endl;
};

void Texture::OutputSingleChannalImage(const std::string& inPath)
{
	FilePath filePath(inPath);

	if (!std::filesystem::exists(inPath))
	{
		std::cout << "open your eyes then input a valid file path." << std::endl;
		return;
	}

	std::string temp = filePath.m_filePath + "/" + filePath.m_fileName;

	// cv初始化什么的
	cv::Mat originalImg;
	originalImg = cv::imread(temp.c_str());
	if (originalImg.empty())
	{
		std::cout << "input a valid path you dick head!" << std::endl;
		/*return 0;*/
	}
	cv::Mat grayImg;

	try {
		cv::cvtColor(originalImg, grayImg, cv::COLOR_BGR2GRAY);
	}
	catch (cv::Exception& e)
	{
		const char* err = e.what();
		std::cerr << "this pic is fucking toxic! " << err << std::endl;
		return;
	}

	//cv::imshow("gray scale result", grayImg);

	//creat directory and file
	std::string fileName = std::filesystem::path(temp.c_str()).filename().string();
	std::string fullOutputPath = filePath.CombineFilePath();

	cv::imwrite(fullOutputPath, grayImg);

	//std::cout << "succesfully convert file to "<< fullOutputPath << std::endl;

	//cv::waitKey(0);

	//int width, height, nrChannels;
	//unsigned char* data = NULL;
	////data = stbi_load(inPath.c_str(), &width, &height, &nrChannels, 4);

	//unsigned int outputDataMem = 512 * 512 * 1;
	//unsigned char* outData = (unsigned char*)malloc(outputDataMem);

	//stbir_resize(data, width, height, nrChannels, outData, width, height, 1, STBIR_TYPE_UINT8, nrChannels, STBIR_ALPHA_CHANNEL_NONE, 0,
	//    STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
	//    STBIR_FILTER_BOX, STBIR_FILTER_BOX,
	//    STBIR_COLORSPACE_SRGB, nullptr
	//            );

	//stbir_resize_uint8(data, width, height, 0,
	//    outData, 512, 512, 0, nrChannels);

	//std::filesystem::create_directory(outPath);
	//std::filesystem::
	//stbiw__write_pixel(data,)

	//if (comp < 0 || comp> 4) {
	//    std::cout << "input channel Out of range." << std::endl;
	//}
	//else {
	//    //stbi_write_png(outPath.c_str(), width, height, comp, data, 0);
	//    //stbi_write_jpg(outPath.c_str(), 512, 512, comp, data, 100);

	//    Sleep(500);
	//    std::cout << "output single Channel" << std::endl;

	//}

	//cv::Mat img(600, 800, CV_8UC3, cv::Scalar(100, 250, 30));

	//stbi_image_free(data);
	//stbi_image_free(outData);
};

void Texture::SearchSimilarImage(const std::string& inPath, const std::string& inPath2, float min, float max, int numFeature, float rejectRatio, std::map<float, std::string>& SimilarImages)
{
	FilePath filePath(inPath);

	if (!std::filesystem::exists(inPath))
	{
		std::cout << "open your eyes then input a valid file path." << std::endl;
	}

	std::string temp = filePath.m_filePath + "/" + filePath.m_fileName;

	FilePath filePath2(inPath2);

	if (!std::filesystem::exists(inPath2))
	{
		std::cout << "open your eyes then input a valid file path." << std::endl;
	}

	std::string temp2 = filePath2.m_filePath + "/" + filePath2.m_fileName;

	// cv初始化什么的
	cv::Mat originalImg;
	cv::Mat originalImg2;
	originalImg = cv::imread(temp.c_str());
	//cv::cvtColor(originalImg, originalImg, cv::COLOR_BGR2GRAY);

	originalImg2 = cv::imread(temp2.c_str());
	//cv::cvtColor(originalImg2, originalImg2, cv::COLOR_BGR2GRAY);

	if (originalImg.empty() || originalImg2.empty())
	{
		std::cout << "input a valid path you dick head!" << std::endl;
		/*return 0;*/
	}

	std::cout << inPath2 << std::endl;

	//SIFT探测器初始化
	int numfeature = numFeature;

	cv::Ptr<cv::SIFT> dect = cv::SIFT::create(numfeature);
	std::vector<cv::KeyPoint> keyPoints;
	std::vector<cv::KeyPoint> keyPoints2;
	dect->detect(originalImg, keyPoints);
	dect->detect(originalImg2, keyPoints2);

	cv::Mat outputImage, outputImage2;

	dect->compute(originalImg, keyPoints, outputImage);
	dect->compute(originalImg2, keyPoints2, outputImage2);

	//cv::drawKeypoints(originalImg, keyPoints, outputImage);
	//cv::drawKeypoints(originalImg2, keyPoints2, outputImage2);

	//匹配
	cv::BFMatcher matcher(cv::NORM_L2);

	try
	{
		//knn match
		std::vector < std::vector<cv::DMatch> > knnMatches;
		matcher.knnMatch(outputImage, outputImage2, knnMatches, 2);

		std::cout << knnMatches.size() << std::endl;

		std::vector<cv::DMatch> goodMatches;
		for (int i = 0; i < knnMatches.size(); i++)
		{
			if (knnMatches[i][0].distance / knnMatches[i][1].distance > rejectRatio)
				continue;
			goodMatches.push_back(knnMatches[i][0]);
		}
		std::cout << goodMatches.size() << std::endl;

		cv::Mat result;
		//goodMatches.resize(100);
		cv::drawMatches(originalImg, keyPoints, originalImg2, keyPoints2, goodMatches, result, cv::Scalar(255, 255, 0), cv::Scalar::all(-1));

		float similarity = float(goodMatches.size()) / float(knnMatches.size());

		std::cout << "fucking knnmatch similarity: " << similarity << std::endl;

		//cv::imshow("match result", result);

		//cv::waitKey(0);

		//std::vector<cv::Point2f> goodKeyPoints1, goodKeyPoints2;
		//for (int i = 0; i < goodMatches.size(); i++)
		//{
		//	goodKeyPoints1.push_back(keyPoints[goodMatches[i].trainIdx].pt);
		//	goodKeyPoints2.push_back(keyPoints2[goodMatches[i].trainIdx].pt);
		//}

		//cv::Mat H = cv::findHomography(goodKeyPoints2, goodKeyPoints1);
		//cv::warpPerspective(originalImg, originalImg , H, originalImg.size(), cv::INTER_NEAREST);
		SimilarImages[similarity] = inPath2;

		std::cout << SimilarImages[3] << std::endl;
	}
	catch (cv::Exception& e)
	{
		const char* err = e.what();
		std::cerr << "this pic is fucking toxic! " << err << std::endl;
	}

	////match
	//{
	//	std::vector<cv::DMatch> matches;
	//	matcher.match(outputImage, outputImage2, matches);

	//	std::sort(matches.begin(), matches.end());

	//	double min_dist = 1000;
	//	double max_dist = 0;
	//	/*
	//	//通过循环更新距离，距离越小越匹配
	//	for (int i = 1; i < outputImage.rows; ++i)
	//	{
	//		//通过循环更新距离，距离越小越匹配
	//		double dist = matches[i].distance;
	//		if (dist > max_dist)
	//			max_dist = dist;
	//		if (dist < min_dist)
	//			min_dist = dist;
	//	}

	//	//匹配结果筛选
	//	std::vector<cv::DMatch> goodMatches;
	//	for (int i = 0; i < matches.size(); ++i)
	//	{
	//		double dist = matches[i].distance;
	//		if (dist < 2 * min_dist)
	//			goodMatches.push_back(matches[i]);
	//	}
	//	*/
	//	//匹配特征点绘制
	//	cv::Mat result;
	//	matches.resize(100);
	//	cv::drawMatches(originalImg, keyPoints, originalImg2, keyPoints2, matches, result, cv::Scalar(255, 255, 0), cv::Scalar::all(-1));

	//	cv::imshow("match result", result);

	//	std::cout << inPath << std::endl;
	//}
}

void Texture::outputText() {
	while (true)
		std::cout << "1/n";
}