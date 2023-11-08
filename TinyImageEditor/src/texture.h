#pragma once

#include <iostream>
#include <map>
#include <filesystem>
#include "imgui.h"

class Texture {
public:
	ImTextureID image_id;
	unsigned int m_textureID;

private:
	const char* path;
	const char* fileName;
	unsigned char* m_LocalBuffer;
	int width, height, nrChannels;

public:
	Texture(const std::string& path);
	~Texture();

	//unsigned char* LoadSingleImage(const std::string& inPath);

	static void ResizeImage(const std::string& inPath, const std::string& outPath, int outputChannel[]);

	static void OutputSingleChannalImage(const std::string& inPath);

	static void outputText();

	static void SearchSimilarImage(const std::string& inPath, const std::string& inPath2, float min, float max, int numFeature, float rejectRatio, std::map<float, std::string>& SimilarImages);
};
