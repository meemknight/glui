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
		float fontSize = 0.f;
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

	constexpr float pressDownSize = 0.04f;
	constexpr float shadowSize = 0.2f;
	constexpr float outlineSize = 0.02f;
	constexpr float textFit  = 0.8f;
	
	//glm::vec4 getShadowPos(glm::vec4 transform)
	//{
	//	auto shadow = transform;
	//	shadow.y += shadow.w * shadowDisplacement;
	//	shadow.x -= shadow.z * shadowDisplacement;
	//	return shadow;
	//}

	void splitTransforms(glm::vec4& down, glm::vec4& newTransform, glm::vec4 transform)
	{
		down = transform;
		newTransform = transform;
		float border = shadowSize * std::min(transform.w, transform.z);
		down.w = border;
		newTransform.w -= border;
		down.y += newTransform.w;
	}

	glm::vec4 stepColorUp(glm::vec4 color, float perc)
	{
		glm::vec4 inversColor = glm::vec4(1, 1, 1, 1) - color;
		return inversColor * perc + color;
	}
	glm::vec4 stepColorDown(glm::vec4 color, float perc)
	{
		color.r *= perc;
		color.g *= perc;
		color.b *= perc;
		return color;
	}

	void renderFancyBox(gl2d::Renderer2D& renderer, glm::vec4 transform, glm::vec4 color, gl2d::Texture t, bool hovered, bool clicked)
	{
		float colorDim = 0.f;
		if (hovered)
		{
			//colorDim += 0.2f;
			if (clicked)
			{
				colorDim += 0.1f;
			}
		}

		auto newColor = stepColorUp(color, colorDim);
		auto lightColor = stepColorUp(newColor, 0.02);
		auto outlineColor = stepColorUp(newColor, 0.3);
		auto darkColor = stepColorDown(newColor, 0.5f);
		auto darkerColor = stepColorDown(newColor, 0.25f);

		glm::vec4 colorVector[4] = {darkColor, darkColor, lightColor, lightColor};

		if (t.id == 0)
		{
			if (hovered)
			{
				float calculatedOutline = outlineSize * std::min(transform.w, transform.z);
				renderer.renderRectangle(transform, outlineColor);
				transform.x	+= calculatedOutline;
				transform.y	+= calculatedOutline;
				transform.z -= calculatedOutline * 2;
				transform.w -= calculatedOutline * 2;
			}

			glm::vec4 middle = {};
			glm::vec4 down = {};
			splitTransforms(down, middle, transform);
			renderer.renderRectangle(middle, colorVector);
			renderer.renderRectangle(down, darkerColor);

		}
		else
		{
			renderer.renderRectangle(transform, {}, 0.f, t, newColor);
		}
	}

	void renderFrame(gl2d::Renderer2D& renderer, gl2d::Font& font, glm::ivec2 mousePos, bool mouseClick, bool mouseHeld, bool mouseReleased, bool escapeReleased)
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
					auto transformDrawn = widget.transform;
					bool hovered = 0;
					bool clicked = 0;

					if (aabb(widget.transform, input.mousePos))
					{
						hovered = true;
						if (input.mouseHeld)
						{
							clicked = true;
							transformDrawn.y += transformDrawn.w * pressDownSize;
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

					renderFancyBox(renderer, transformDrawn, widget.colors, widget.texture, hovered, clicked);

					glm::vec2 pos = glm::vec2(transformDrawn);
					pos.x += transformDrawn.z / 2.f;
					pos.y += transformDrawn.w / 2.f;

					float s = 1.5;
					auto size = renderer.getTextSize(i.first.c_str(), font, s);

					float newSx = s * (transformDrawn.z*textFit) / size.x;
					float newSy = s * (transformDrawn.w*textFit) / size.y;

					float newS = std::min(newSx, newSy);

					renderer.renderText(pos, i.first.c_str(), font, Colors_White, newS);

					break;
				}

			}

			widget.lastFrameData = input;
		}


		renderer.currentCamera = camera;

	}

	bool Button(std::string name, const gl2d::Rect transform, const gl2d::Color4f colors, float fontSize, const gl2d::Texture texture)
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
			widget.fontSize = fontSize;

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
			find->second.transform = transform;
			find->second.colors = colors;
			find->second.texture = texture;
			find->second.fontSize = fontSize;

			return find->second.returnFromUpdate;
		}

	}


};
