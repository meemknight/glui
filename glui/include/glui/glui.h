//////////////////////////////////////////////////
//gl2d.h				1.0.0
//Copyright(c) 2023 Luta Vlad
//https://github.com/meemknight/glui
//
//
//	dependences: gl2d, glew, glm, stb_image, stb_trueType
//
//
//////////////////////////////////////////////////


#pragma once
#include "gl2d/gl2d.h"
#include <string>
#include <unordered_map>

namespace glui
{

	struct RendererUi
	{

		void renderFrame(
			gl2d::Renderer2D &renderer,
			gl2d::Font &font,
			glm::ivec2 mousePos,
			bool mouseClick,
			bool mouseHeld,
			bool mouseReleased,
			bool escapeReleased,
			const std::string &typedInput,
			float deltaTime
		);

		bool Button(std::string name,
			const gl2d::Color4f colors, const gl2d::Texture texture = {});

		void Texture(int id, gl2d::Texture t, gl2d::Color4f colors = {1,1,1,1}, glm::vec4 textureCoords = {0,1,1,0});

		bool ButtonWithTexture(int id, gl2d::Texture t, gl2d::Color4f colors = {1,1,1,1}, glm::vec4 textureCoords = {0,1,1,0});

		bool Toggle(std::string name,
			const gl2d::Color4f colors, bool *toggle, const gl2d::Texture texture = {}, const gl2d::Texture overTexture = {});

		//returns true if you should render it, clicked is optional
		bool CustomWidget(int id, glm::vec4 *transform, bool *hovered = 0, bool *clicked = 0);

		void Text(std::string name,
			const gl2d::Color4f colors);

		void InputText(std::string name,
			char *text, size_t textSizeWithNullChar, gl2d::Color4f color = {0,0,0,0}, const gl2d::Texture texture = {});

		void sliderFloat(std::string name, float *value, float min, float max,
			gl2d::Texture sliderTexture = {}, gl2d::Color4f sliderColor = {1,1,1,1},
			gl2d::Texture ballTexture = {}, gl2d::Color4f ballColor = {1,1,1,1});

		void sliderInt(std::string name, int *value, int min, int max,
			gl2d::Texture sliderTexture = {}, gl2d::Color4f sliderColor = {1,1,1,1},
			gl2d::Texture ballTexture = {}, gl2d::Color4f ballColor = {1,1,1,1});

		void colorPicker(std::string name, float *color3Component, gl2d::Texture sliderTexture = {},
			gl2d::Texture ballTexture = {});

		void newColum(int id);

		void PushId(int id);

		void PopId();

		void BeginMenu(std::string name, const gl2d::Color4f colors, const gl2d::Texture texture);
		void EndMenu();

		void Begin(int id);
		void End();

		struct Internal
		{
			struct InputData
			{
				glm::ivec2 mousePos = {};
				bool mouseClick = 0;
				bool mouseHeld = 0;
				bool mouseReleased = 0;
				bool escapeReleased = 0;
			};

			struct Widget
			{
				int type = 0;
				bool justCreated = true;
				bool usedThisFrame = 0;
				InputData lastFrameData = {};
				gl2d::Color4f colors = Colors_White;
				gl2d::Color4f colors2 = Colors_White;
				gl2d::Texture texture = {};
				gl2d::Texture textureOver = {};
				glm::vec4 textureCoords = {};
				bool returnFromUpdate = 0;
				bool customWidgetUsed = 0;
				void *pointer = 0;
				bool clicked = 0; //todo for all?
				bool hovered = 0;
				float min = 0;
				float max = 0;
				int minInt = 0;
				int maxInt = 0;
				glm::vec4 returnTransform = {};//todo mabe for every widget?

				struct PersistentData
				{
					bool sliderBeingDragged = 0;
					bool sliderBeingDragged2 = 0;
					bool sliderBeingDragged3 = 0;
				}pd;

				size_t textSize = 0;
			};
			

			std::vector<std::pair<std::string, Widget>> widgetsVector;

			std::unordered_map<std::string, Widget> widgets;

			std::unordered_map<int, std::vector<std::string>> allMenuStacks;

			std::string idStr;
		}internal;

	};


	void defaultErrorFunc(const char* msg);
	using errorFuncType = decltype(defaultErrorFunc);

	

};