#include "GLFW/glfw3.h"
#include "opencv.hpp"
#include <opencv2/core.hpp>
#include <filesystem>

//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include "texture.h"
#include "windows.h"

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
    m_LocalBuffer(nullptr),
    width(0),height(0),nrChannels(0)
{
    cv::Mat m_texture;
    m_texture = cv::imread(path.c_str(),cv::IMREAD_ANYCOLOR);
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

int Texture::a = 0;


void Texture::OutputSingleChannalImage(const std::string& inPath, const std::string& outPath, int stride, int comp)
{
    std::string outputDir = "D:/output";

    cv::Mat originalImg;
    originalImg = cv::imread(inPath.c_str()); // �ĳ��Լ���ͼƬ·��
    if (originalImg.empty())
    {
        std::cout << "��ȷ��ͼ���ļ����Ƿ���ȷ" << std::endl;
        /*return 0;*/
    }
    cv::Mat grayImg;
    cv::cvtColor(originalImg, grayImg, cv::COLOR_BGR2GRAY);

    cv::imshow("gray scale result", grayImg);

    //outputDir + "grayScaleImage.png";

    //creat directory and file
    std::filesystem::path outputPath(outputDir);
    try 
    {
        std::filesystem::create_directory(outputPath);
        std::cout << "Successfully created directory " << outputPath << std::endl;
    }
    catch (std::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }


    std::string fileName = std::filesystem::path(inPath).filename().string();
    std::string fullOutputPath = outputDir + "/" + fileName;

    cv::imwrite(fullOutputPath, grayImg);



    std::cout << "succesfully convert file to "<< fullOutputPath << std::endl;

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
