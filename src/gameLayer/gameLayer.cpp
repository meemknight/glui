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

struct GameData
{

}gameData;


bool initGame()
{
	renderer.create();
	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	texture.loadFromFile(RESOURCES_PATH "test.jpg");

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


	if (glui::Button("test", {100,100,100,100}, Colors_Orange, 1.f)) 
	{
		std::cout << "yay\n";
	};

	glui::Button("test2\ntest2", {300,100,30,30}, Colors_Red, 1.f);

	glui::Button("test3", {100,300,100,30}, Colors_Turqoise, 1.f);

	glui::Button("test4", {300,300,30,50}, Colors_Magenta, 1.f);

	glui::Button("test5", {200,400,130,30}, Colors_Gray, 1.f);
	glui::Button("test6", {200,435,130,30}, Colors_Gray, 1.f);
	glui::Button("test7", {200,470,130,30}, Colors_Gray, 1.f);
	glui::Button("test8", {200,505,130,30}, Colors_Gray, 1.f);
	glui::Button("test9", {200,540,130,30}, Colors_Gray, 1.f);


#pragma region set finishing stuff

	glui::renderFrame(renderer, font, platform::getRelMousePosition(),
		platform::isLMousePressed(), platform::isLMouseHeld(), platform::isLMouseReleased(),
		platform::isKeyReleased(platform::Button::Escape));

	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
