#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#include <iostream>
#include <string>
#include <unordered_map>
#include "SDL3/SDL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "opencv2/opencv.hpp"
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/serial_port.hpp"
#include "asio/ts/internet.hpp"

typedef unsigned char byte;

//cap_msmf.cpp.obj

SDL_Window* mainWindow;
SDL_Renderer* mainWinRender;

std::unordered_map<size_t, size_t> cvToSDLPixelForm;

int* mainWindowWidth;
int* mainWindowHeight;
std::string filePath = "/mnt/c/Users/bub48/source/repos/LEDPanelDesktopClient/res/test.png";


SDL_Texture* imgPrevSDL = nullptr;
ImTextureID imgPrevIMGUI;

cv::Mat originalImage;
cv::Mat previewImage;
cv::Mat sendImage;

asio::io_context ioCTX;
asio::serial_port* ser = nullptr;

std::string portID = "COM7";
unsigned int baudRate = 9600;

struct Dimensions {
	int w;
	int h;
};
//asio::serial_port ser;

Dimensions outImageDimensions{ 0, 0 };
Dimensions inImageDimensions{ 0, 0 };

char tempPath [1000] = "File path.";
char tempPortId [100] = "Serial port";

bool sendData = false;
bool isImageLoaded = false;
bool updateScale = true;
bool isPortOpen = false;
bool canOpenPort = false;


void InitSDL();
void InitIMGUI();
void Render();
void InitPixelFormats();
void UpdatePrevImageScale(Dimensions);
bool CheckPathValid(std::string);
bool CheckPort(std::string);

int main(int argc, char* argv[]) {
	////set text box values to all spaces
	//for (int i = 0; i < 1000; i++) {
	//	tempPath[i] = ' ';
	//}
	//for (int i = 0; i < 100; i++) {
	//	tempPortId[i] = ' ';
	//}

	mainWindowWidth = (int*)malloc(sizeof(int));
	mainWindowHeight = (int*)malloc(sizeof(int));

	previewImage = cv::Mat::zeros(cv::Size(200, 200), CV_8UC3);

	InitSDL();
	SDL_GetWindowSize(mainWindow, mainWindowWidth, mainWindowHeight);
	InitIMGUI();

	inImageDimensions.w = originalImage.rows;
	inImageDimensions.h = originalImage.cols;
	outImageDimensions = { 20,20 };

	bool isOpen;
	
	bool run = true;
	SDL_Event evt;
	while (run) {
		while (SDL_PollEvent(&evt)) {
			ImGui_ImplSDL3_ProcessEvent(&evt);
			switch (evt.type) {
				case SDL_EVENT_QUIT:
					run = false;
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					SDL_GetWindowSize(mainWindow, mainWindowWidth, mainWindowHeight);
					break;
			}
		}
		if (isImageLoaded && updateScale)
			UpdatePrevImageScale(outImageDimensions);
		char InDims[20];
		if (canOpenPort) {
			asio::error_code ec;
			if (ser != nullptr)
				delete ser;
			ser = new asio::serial_port(ioCTX, portID);

			ser->set_option(asio::serial_port_base::baud_rate(9600));
			ser->set_option(asio::serial_port_base::character_size(8));
			ser->set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
			ser->set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
			ser->set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

			//since reseting generates random bytes i want to clear that out before i send and recieve data
			//char resetBuff[100];
			//std::cout << "clearing reset buff" << std::endl;
			//ser->read_some(asio::buffer(resetBuff), ec);

			//get the dimentions
			ser->write_some(asio::buffer("GET"), ec);

			canOpenPort = false;
			isPortOpen = true;
			ser->read_some(asio::buffer(InDims), ec);
			outImageDimensions.w = InDims[0];
			outImageDimensions.h = InDims[0];
		}
		Render();
		if (outImageDimensions.w <= 20) {
			outImageDimensions.w = 20;
		}
		if (outImageDimensions.h <= 20) {
			outImageDimensions.h = 20;
		}
		//asio::write(ser, asio::buffer(bufferTest));
		//
		//char inBuff[29];
		//try {
		//	ser.read_some(asio::buffer(inBuff));
		//}
		//catch (std::exception e) {
		//	std::cout << "couldn't read buffer" << std::endl;
		//}
		//std::cout << inBuff << std::endl;
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
void InitSDL() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Failed to init SDL" << std::endl;
		exit(EXIT_FAILURE);
	}
	mainWindow = SDL_CreateWindow("LED Desktop Client", 1600, 900, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	SDL_ShowWindow(mainWindow);
	mainWinRender = SDL_CreateRenderer(mainWindow, NULL);
	SDL_SetRenderDrawColor(mainWinRender, 0, 0, 0, 255);
	
	std::cout << "Init Success" << std::endl;
}
void InitIMGUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imguiIO = ImGui::GetIO();
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplSDL3_InitForSDLRenderer(mainWindow, mainWinRender);
	ImGui_ImplSDLRenderer3_Init(mainWinRender);
}


void Render() {
	SDL_RenderClear(mainWinRender);
	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui::NewFrame();
	bool open = true;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(*mainWindowWidth / 2, *mainWindowHeight));
	ImGui::Begin("Dock panel", & open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text("preview image goes here");
		std::string str = "Output Dimensions:\t";
		str += std::to_string(outImageDimensions.w);
		str += "x";
		str += std::to_string(outImageDimensions.h);
		ImGui::Text(str.c_str());
		/*bool updateW = ImGui::InputInt("Out Width", &outImageDimensions.w);
		bool updateH = ImGui::InputInt("Out Height", &outImageDimensions.h);
		updateScale = (updateW || updateH);*/
		
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::BeginDisabled(isImageLoaded && isPortOpen);
		if (!sendData)
			sendData = ImGui::Button("Begin Send Data");
		else
			sendData = !ImGui::Button("End Send Data");
		ImGui::EndDisabled();
		ImGui::PopStyleVar();
		ImGui::Image(imgPrevIMGUI, ImVec2(200, 200));
		
	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(*mainWindowWidth/2, 0));
	ImGui::SetNextWindowSize(ImVec2(*mainWindowWidth / 2, *mainWindowHeight));
	ImGui::Begin("Dock panel 2", & open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
	{
		if (sendData)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::BeginDisabled(sendData);
		ImGui::Text("file selector goes here");
		if (ImGui::InputText("Type file directory.", tempPath, 1000)) {
			std::cout << tempPath << std::endl;
			filePath = std::string(tempPath);
			isImageLoaded = CheckPathValid(filePath);
			updateScale = isImageLoaded;
		}
		if (ImGui::InputText("Type serial port ID here.", tempPortId, 100)) {
			std::cout << "Check port" << std::endl;
			portID = tempPortId;
			canOpenPort = CheckPort(portID);
		}
		
			ImGui::EndDisabled();
			if (sendData)
				ImGui::PopStyleVar();
	}
	ImGui::End();
	ImGui::Render();

	//SDL_RenderFillRect(mainWinRender, mainScreenRect);
	//SDL_RenderRect(prevPanRender, prevScreenRect);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), mainWinRender);
	SDL_RenderPresent(mainWinRender);
	
}
void UpdatePrevImageScale(Dimensions newDims) {
//I thought i'd have to create a map from opencv pixel formats to SDL pixel formats manually but thankfully opencv debaults to BGR with 8 bpp
//But since SDL_PIXELFORMAT_BGR888 is depracated in this version i had to use SDL_PIXELFORMAT_BGR24
//one is column major and the other is row major so I had to swap rows and columns
	cv::resize(originalImage, sendImage, cv::Size(newDims.w, newDims.h), 0.0, 0.0, cv::INTER_NEAREST_EXACT);
	cv::resize(sendImage, previewImage, cv::Size(200, 200), 0.0, 0.0, cv::INTER_NEAREST_EXACT);
	SDL_Surface* tempSurf = SDL_CreateSurfaceFrom(200, 200, SDL_PIXELFORMAT_BGR24, previewImage.data, 200*3);
	std::cout << SDL_GetError() << std::endl;
	imgPrevSDL = SDL_CreateTextureFromSurface(mainWinRender, tempSurf);
	if (imgPrevSDL == nullptr) {
		std::cout << "Couldn't load image." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << SDL_GetError() << std::endl;
	SDL_DestroySurface(tempSurf);
	imgPrevIMGUI = (ImTextureID)imgPrevSDL;
}

bool CheckPathValid(std::string path) {
	cv::Mat checkPath = cv::imread(path);
	if (checkPath.empty())
		return false;
	originalImage = cv::imread(path);
	return true;
}
bool CheckPort(std::string port) {
	try {
		if (ser != nullptr)
			ser->close();
		asio::serial_port testPort(ioCTX, port);
		testPort.close();
		return true;
	}
	catch (std::exception e) {
		std::cout << "Cannot open " << port << std::endl;
		isPortOpen = false;
		return false;
	}
	return false;
}