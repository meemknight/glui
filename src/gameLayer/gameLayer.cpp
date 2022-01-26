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


	glui::Text("Terarria", Colors_Gray);

	if (glui::Button("Play", Colors_Transparent, texture))
	{
		//play
	}

	glui::BeginMenu("settings", Colors_Transparent, texture);
		glui::BeginMenu("volume settings", Colors_Transparent, texture);
			static bool sound = true;
			static bool music = true;
			glui::Toggle("sound", Colors_Gray, &sound, texture, tick);
			glui::Toggle("music", Colors_Gray, &music, texture, tick);
		glui::EndMenu();

		glui::BeginMenu("video settings", Colors_Transparent, texture);
			static bool vSync = true;
			static bool shadows = true;
			glui::Toggle("vSync", Colors_Gray, &vSync, texture, tick);
			glui::Toggle("shadows", Colors_Gray, &shadows, texture, tick);
		glui::EndMenu();
	glui::EndMenu();

	glui::BeginMenu("create new world", Colors_Transparent, texture);
		glui::Text("Enter world name", Colors_Gray);
		static char text[15];
		glui::InputText("input", text, sizeof(text));
		glui::Button("create", Colors_Transparent, texture);
	glui::EndMenu();
	glui::Button("Exit", Colors_Transparent, texture);


	//std::cout << platform::isKeyReleased(platform::Button::Q) << "\n";

#pragma region set finishing stuff

	glui::renderFrame(renderer, font, platform::getRelMousePosition(),
		platform::isLMousePressed(), platform::isLMouseHeld(), platform::isLMouseReleased(),
		platform::isKeyReleased(platform::Button::Escape), platform::getTypedInput(), deltaTime);

	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
