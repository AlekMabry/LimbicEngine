#pragma once
#ifndef HUD_HPP
#define HUD_HPP

#include <iostream>
#include <map>
#include "common.hpp"
#include "shaderLoader.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint     textureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class hud
{
private:
	GLFWwindow* window;
	GLuint shd_crosshair_dynamic;
	GLuint shd_font_singlecolor;

	GLuint gl_vao_primaryFont;
	GLuint gl_vbo_primaryFont;

	std::map<GLchar, Character> Characters;

public:
	hud();
	void loadFont();
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif
