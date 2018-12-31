#ifndef RENDER_HPP
#define RENDER_HPP

#include "common.hpp"

struct glyph
{
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::ivec2 offset;
	GLint advance;
};

struct font
{
	short size;
	short lineHeight;
	short base;
	short w;
	short h;
	char texture[16];
};

class text
{
private:
	font primaryFont;
	std::map<GLchar, glyph> primaryFontGlyphInfo;

	GLuint gl_tex_primaryFont;
	GLuint shd_font_singlecolor;
	GLuint gl_vao_primaryFont;
	GLuint gl_vbo_primaryFont;
public:
	text();
	void loadFont(char* fname);
	void renderText(std::string text, GLuint x, GLuint y, float size, glm::vec3 color);
};

#endif
