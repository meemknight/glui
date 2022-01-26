#include "gameLayer.h"
#include "gl2d/gl2d.h"
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "glui/glui.h"

gl2d::Renderer2D renderer;

gl2d::Font font;
gl2d::Texture texture;
gl2d::Texture tick;

struct GameData
{

}gameData;


bool initGame()
{
	renderer.create();
	//font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	font.createFromFile(RESOURCES_PATH "font/ANDYB.TTF");
	texture.loadFromFile(RESOURCES_PATH "ui.png", true);
	tick.loadFromFile(RESOURCES_PATH "tick.png", true);

	if(!platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	glui::gluiInit();

	return true;
}

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w= platform::getWindowSizeX();
	h = platform::getWindowSizeY();
	
	renderer.updateWindowMetrics(w, h);
	renderer.clearScreen(gl2d::Color4f(0.2,0.2,0.3, 1));
#pragma endregion

	ImGui::ShowDemoWindow();


	if (glui::Button("test", {0,0,0,0}))
	{
		std::cout << "yay\n";
	};

	static char text[30];
	static char text2[30];

	glui::InputText("test2\ntest2", text, sizeof(text));
	glui::InputText("test2\ntest3", text2, sizeof(text2), Colors_Gray, texture);

	glui::Button("test3##hash", Colors_Turqoise, texture);

	glui::Button("test3##repeatHash", Colors_Magenta, texture);

	glui::BeginMenu("settings", Colors_Yellow, texture);
		glui::Button("test5", Colors_Red, texture);
		glui::Button("test7", Colors_Red, texture);
		glui::BeginMenu("menu2", Colors_Yellow, texture);
			glui::Button("test5", Colors_Orange, texture);
			glui::Button("test7", Colors_Orange, texture);
		glui::EndMenu();
	glui::EndMenu();
	
	glui::Text("test7", Colors_Turqoise);
	

	glui::Button("test8", Colors_Gray, texture);
	glui::Button("test9", Colors_Gray, texture);

	static bool t = false;
	glui::Toggle("toggle", Colors_Gray, &t, texture, tick);

	//std::cout << platform::isKeyReleased(platform::Button::Q) << "\n";

#pragma region set finishing stuff

	glui::renderFrame(renderer, font, platform::getRelMousePosition(),
		platform::isLMousePressed(), platform::isLMouseHeld(), platform::isLMouseReleased(),
		platform::isRMouseReleased(), platform::getTypedInput(), deltaTime);

	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
