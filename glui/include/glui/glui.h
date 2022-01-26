#pragma once
#include "gl2d/gl2d.h"
#include <string>


namespace glui
{

	void gluiInit();

	void defaultErrorFunc(const char* msg);
	using errorFuncType = decltype(defaultErrorFunc);

	void renderFrame(
	gl2d::Renderer2D &renderer,
	gl2d::Font &font,
	glm::ivec2 mousePos,
	bool mouseClick,
	bool mouseHeld,
	bool mouseReleased,
	bool escapeReleased
	);


	bool Button(std::string name, const gl2d::Rect transform,
		const gl2d::Color4f colors, const gl2d::Texture texture = {});

	bool Toggle(std::string name, const gl2d::Rect transform,
		const gl2d::Color4f colors, bool* toggle, const gl2d::Texture texture = {}, const gl2d::Texture overTexture = {});

};