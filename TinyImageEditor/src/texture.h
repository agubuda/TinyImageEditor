#pragma once

#include <iostream>
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
    static int a;


public:
    Texture(const std::string& path);
    ~Texture();

    //unsigned char* LoadSingleImage(const std::string& inPath);

    static void ResizeImage(const std::string& inPath, const std::string& outPath, int outputChannel[]);

    static void OutputSingleChannalImage(const std::string& inPath);
};


