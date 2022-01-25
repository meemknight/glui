#include "glui/glui.h"
#include "gl2d/gl2d.h"
#include <unordered_map>
#include <iostream>

namespace glui
{


	void defaultErrorFunc(const char* msg)
	{
		std::cerr << "glui error: " << msg << "\n";
	}

	static errorFuncType* errorFunc = defaultErrorFunc;

	errorFuncType* setErrorFuncCallback(errorFuncType* newFunc)
	{
		auto a = errorFunc;
		errorFunc = newFunc;
		return a;
	}

	void gluiInit()
	{


	}

	enum widgetType
	{
		none = 0,
		button,
	};

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
		gl2d::Rect transform = {};
		gl2d::Color4f colors = Colors_White;
		gl2d::Texture texture = {};
		bool returnFromUpdate = 0;
	};


	bool aabb(glm::vec4 transform, glm::vec2 point)
	{
		if (
			point.x >= transform.x &&
			point.y >= transform.y &&
			point.x <= transform.x + transform.z &&
			point.y <= transform.y + transform.w
			)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::unordered_map<std::string, Widget> widgets;

	void renderFrame(gl2d::Renderer2D& renderer, glm::ivec2 mousePos, bool mouseClick, bool mouseHeld, bool mouseReleased, bool escapeReleased)
	{

		auto camera = renderer.currentCamera;
		renderer.currentCamera.setDefault();

		//clear unused data
		{
			std::unordered_map<std::string, Widget> widgets2;
			widgets2.reserve(widgets.size());
			for (auto& i : widgets)
			{
				if (i.second.usedThisFrame)
				{
					widgets2.insert(i);
				}
			}
			widgets = std::move(widgets2);
		}

		InputData input;
		input.mousePos = mousePos;
		input.mouseClick = mouseClick;
		input.mouseHeld = mouseHeld;
		input.mouseReleased = mouseReleased;
		input.escapeReleased = escapeReleased;

		for (auto& i : widgets)
		{
			auto& widget = i.second;
			widget.usedThisFrame = false;
			widget.justCreated = false;

			switch (widget.type)
			{
				case widgetType::button:
				{
					float colorDim = 1.f;
					
					if (aabb(widget.transform, input.mousePos))
					{
						colorDim -= 0.2f;
						if (input.mouseHeld)
						{
							colorDim -= 0.2f;
						}
					}

					if (input.mouseReleased && aabb(widget.transform, input.mousePos))
					{
						widget.returnFromUpdate = true;
					}
					else
					{
						widget.returnFromUpdate = false;
					}

					auto newColor = widget.colors;
					newColor.r *= colorDim;
					newColor.g *= colorDim;
					newColor.b *= colorDim;

					if (widget.texture.id == 0)
					{
						renderer.renderRectangle(widget.transform, newColor);
					}
					else
					{
						renderer.renderRectangle(widget.transform, {}, 0.f, widget.texture, newColor);
					}

					break;
				}

			}

			widget.lastFrameData = input;
		}


		renderer.currentCamera = camera;

	}

	bool Button(std::string name, const gl2d::Rect transform, const gl2d::Color4f colors, const gl2d::Texture texture)
	{
		
		auto find = widgets.find(name);
		
		if (find == widgets.end())
		{
			Widget widget = {};
			widget.type = widgetType::button;
			widget.transform = transform;
			widget.colors = colors;
			widget.texture = texture;
			widget.usedThisFrame = true;
			widget.justCreated = true;

			widgets.insert({name, widget});
			return false;
		}
		else
		{
			if (find->second.type != widgetType::button)
			{
				errorFunc("reupdated a widget with a different type");
			}

			if (find->second.usedThisFrame == true)
			{
				errorFunc("used a widget name twice");

			}
			find->second.usedThisFrame = true;

			return find->second.returnFromUpdate;
		}

	}


};
