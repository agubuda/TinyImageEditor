//#include <iostream>
//#include <string.h>
//#include <sstream>
//#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>

#include "application.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "texture.h"
#include "filePath.h"

std::mutex mtx;
static float progressBar;
std::atomic<float> progressBarPercentage(0.0f);

void convertImageProcess(std::vector<std::string> imageList)
{
	for (int i = 0; i < imageList.size(); i++)
	{
		//线程锁
		std::lock_guard<std::mutex> lock(mtx);

		Texture::OutputSingleChannalImage(imageList[i]);

		//改为原子变量
		//progressBar = float(i+1) / float(imageList.size());
		progressBarPercentage.store(float(i + 1) / float(imageList.size()));

		std::cout << "Converted " << i + 1 << " in " << imageList.size() << std::endl;
	}
}

void compareImageProcess(std::string srcPath, std::vector<std::string> imageList, float min, float max, int numFeature, float rejectRatio, std::map<float, std::string>& SimilarImages)
{
	for (int i = 0; i < imageList.size(); i++)
	{
		//线程锁
		std::lock_guard<std::mutex> lock(mtx);

		Texture::SearchSimilarImage(srcPath, imageList[i], min, max, numFeature, rejectRatio, SimilarImages);

		//改为原子变量
		//progressBar = float(i+1) / float(imageList.size());
		//progressBarPercentage.store(float(i + 1) / float(imageList.size()));

		std::cout << "compareing " << i + 1 << " in " << imageList.size() << std::endl;
	}
}

namespace MyApp
{
	ImTextureID imImage_ID;

	void RenderUI(unsigned int m_textureID)
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//control pannal
		ImGui::Begin(u8"Image settings 图片设置");
		ImGui::Button("Image base color");
		static float colorR = 1.0f;
		static float colorG = 1.0f;
		static float colorB = 1.0f;
		ImGui::DragFloat("R", &colorR, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("G", &colorG, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("B", &colorB, 0.01f, 0.0f, 1.0f);
		ImGui::End();

		//image view window
		ImGui::Begin("View port");

		imImage_ID = (GLuint*)m_textureID;

		ImGui::Image(imImage_ID, ImGui::GetContentRegionAvail(), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0), ImVec4(colorR, colorG, colorB, 1), ImVec4(0, 0, 0, 1));

		ImGui::End();

		//file exploer
		ImGui::Begin("Load images");
		ImGui::Text(u8"Load Texture from Path 小白不行");
		//ImGui::InputText("texPath", tex_path_buffer, 128);
		//ImGui::SameLine();

		static int i[2] = { 512 ,512 };
		ImGui::InputInt2("Stride", i);

		if (ImGui::Button("Generate"))
		{
			Texture::ResizeImage("C:\\Users\\container.jpg", "D:\\container_R2.png", i);

			//texture.ResizeImage("C:\\Users\\container.jpg", "D:\\container_resize.jpg");
		}

		//static int stride = 0;
		//static int comp = 1;
		//ImGui::InputInt("Stride", &stride);
		//ImGui::InputInt("Comp", &comp);
		static char str0[128] = "Input image Path here.";
		ImGui::InputText("Image path", str0, IM_ARRAYSIZE(str0));
		//ImGui::InputTextWithHint("Image path", "Image path", str0, IM_ARRAYSIZE(str0), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::Text("Just like C:/Users/liuwangyang/Pictures/icon.jpg");
		if (ImGui::Button("Generate Single"))
		{
			std::string inPath = str0;
			Texture::OutputSingleChannalImage(inPath);
			//texture.ResizeImage("C:\\Users\\container.jpg", "D:\\container_resize.jpg");
		}

		static bool showProgress = false;
		static char str1[128] = "C:/Users/liuwangyang/Pictures/";
		ImGui::InputText("Image dir", str1, IM_ARRAYSIZE(str1));

		if (ImGui::Button("Traversal convert images to grayscale"))
		{
			std::vector<std::string> imageList;
			std::string dirPath = str1;
			FilePath file = dirPath;

			static bool animate = true;

			imageList = file.ListAllFilePathes();

			//for (int i = 0; i < imageList.size(); i++)
			//{
			//    Texture::OutputSingleChannalImage(imageList[i]);
			//    std::cout << "Converted " << i+1  << " in " << imageList.size() << std::endl;

			//    progress += 0.1;
			//    // Animate a simple progress bar
			//    //IMGUI_DEMO_MARKER("Widgets/Plotting/ProgressBar");

			//}

			std::thread(convertImageProcess, imageList).detach();  //传值还是传地址可以优化
			showProgress = true;
			//texture.ResizeImage("C:\\Users\\container.jpg", "D:\\container_resize.jpg");
		}
		if (showProgress)
		{
			ImGui::ProgressBar(progressBarPercentage, ImVec2(0.0f, 0.0f));
			//progressBar += 0.01;
			//std::cout << "fuckkkkkkkk" << progressBar << std::endl;
		}

		//similarity check
		ImGui::Text("Search similar images");

		static int numfeature = 500;
		static float rejectRatio = 0.8f;
		ImGui::InputInt("num feature", &numfeature);
		ImGui::DragFloat("Reject Ratio", &rejectRatio, 0.01f, 0.0f, 1.0f);

		static float matchRange[2] = { 1000 ,0 };
		ImGui::InputFloat2("Min, Max", matchRange);
		static char str2[128] = "D:/Pictures/9f831b27ea15a76998819610f416d984.png";
		static char str3[128] = "D:/Pictures/009804a93f11d1422b474705101a0ea9.png";
		ImGui::InputText("src Image dir", str2, IM_ARRAYSIZE(str2));
		ImGui::InputText("dst Image dir", str3, IM_ARRAYSIZE(str3));
		if (ImGui::Button("Search similar images"))
		{
			std::string imagePath = str2;

			std::string dirPath = str3;

			std::map<float, std::string> similarImages;

			std::vector<std::string> imageList;

			FilePath file = dirPath;

			imageList = file.ListAllFilePathes();

			//std::thread(compareImageProcess, imagePath, imageList, matchRange[1], matchRange[2], numfeature, rejectRatio, similarImages).detach();

			for (int i = 0; i < imageList.size(); i++)
			{
				Texture::SearchSimilarImage(imagePath, imageList[i], matchRange[1], matchRange[2], numfeature, rejectRatio, similarImages);

				std::cout << "compareing " << i + 1 << " in " << imageList.size() << std::endl;
			}

			std::cout << "compareing successful" << std::endl;
			//Texture::SearchSimilarImage(str2, str3, matchRange[1], matchRange[2], numfeature, rejectRatio, similarImages);
		}

		ImGui::End();

		ImGui::End();
	}
}