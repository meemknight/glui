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
gl2d::Texture terrariaTexture;
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
	terrariaTexture.loadFromFile(RESOURCES_PATH "terraria.png");
	tick.loadFromFile(RESOURCES_PATH "tick.png", true);

	if(!platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	glui::gluiInit();

	return true;
}

void render1()
{
	glui::Begin(6996);
		//glui::Text("Terraria", Colors_Gray);
		glui::Texture(0, terrariaTexture);
		glui::Texture(1, terrariaTexture);
		glui::Texture(2, terrariaTexture);

		if (glui::ButtonWithTexture(1, terrariaTexture))
		{
			//play
		}

		static float value = 0;
		static int valueInt = 0;
		glui::sliderFloat("Slider example", &value, -2, 5, texture, {1,1,1,1}, texture);
		glui::sliderInt("Slider example int", &valueInt, -2, 5, texture, {1,1,1,1}, texture);

		glm::vec4 customTransform = {};
		bool clicked = 0;
		bool hovered = 0;
		if (glui::CustomWidget(23, &customTransform, &hovered, &clicked))
		{
			if (clicked)
			{
				renderer.renderRectangle(customTransform, Colors_Blue);
			}
			else if (hovered)
			{
				renderer.renderRectangle(customTransform, Colors_Green);
			}
			else
			{
				renderer.renderRectangle(customTransform, Colors_Red);
			}
		}

		glui::BeginMenu("color test", Colors_Transparent, texture);
		{
			static glm::vec3 color = {};
			static glm::vec3 color2 = {};
			glui::colorPicker("color example", &color[0], texture, texture);
			glui::colorPicker("color example2", &color2[0], texture, texture);

			glui::newColum(0);

			glui::Texture(3, texture, {color, 1});
			glui::Texture(4, texture, {color2, 1});
		
		}
		glui::EndMenu();


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

		glui::BeginMenu("create new world", Colors_Green, texture);
			glui::Text("Enter world name", Colors_Gray);
			static char text[15];
			glui::InputText("input", text, sizeof(text));
			glui::Button("create", Colors_Transparent, texture);
		glui::EndMenu();
		glui::Button("Exit", Colors_Transparent, texture);

	glui::End();
}

void render2()
{
	glui::Begin(100);
		glui::Text("Minicraft", Colors_Blue);

		if (glui::Button("Play", Colors_Green, texture))
		{
			//play
		}

		glui::BeginMenu("settings", Colors_Transparent, texture);
			glui::BeginMenu("vsync settings", Colors_Transparent, texture);
				glui::Text("vsync stuff##test", Colors_White);
			glui::EndMenu();

			glui::BeginMenu("video settings", Colors_Transparent, texture);
				static bool vSync = true;
				static bool shadows = true;
				glui::Toggle("vSync", Colors_Gray, &vSync, texture, tick);
				glui::Toggle("shadows", Colors_Gray, &shadows, texture, tick);
			glui::EndMenu();
		glui::EndMenu();

		glui::BeginMenu("Create new world", Colors_Green, texture);
			glui::Text("Enter world name", Colors_Gray);
			static char text[15];
			glui::InputText("input", text, sizeof(text));
			glui::Button("create", Colors_Transparent, texture);
		glui::EndMenu();
		glui::Button("Exit...", Colors_Gray, texture);
	
	glui::End();


}


bool change = 0;

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

	if (change)
	{
		render2();
	}
	else
	{
		render1();
	}


	if (platform::isKeyReleased(platform::Button::Q))
	{
		change = !change;
	}

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
