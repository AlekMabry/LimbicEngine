#include "text.hpp"
#include "textureLoader.hpp"
#include "shaderLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <map>

text::text()
{
	shd_font_singlecolor = LoadShaders("outpost731/shaders/font_vert.shader", "outpost731/shaders/font_frag.shader");

	glGenVertexArrays(1, &gl_vao_primaryFont);
	glGenBuffers(1, &gl_vbo_primaryFont);
	glBindVertexArray(gl_vao_primaryFont);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vbo_primaryFont);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// "outpost731/fonts/terminal.fnt"
void text::loadFont(char* fname)
{
	std::string cLine;
	std::ifstream fStream(fname, std::ios::in);

	if (fStream.is_open()) {
		std::string line;
		while (std::getline(fStream, line))
		{
			// The line contains one set of info.
			// Split this line by spaces into phrases
			std::istringstream iss(line);
			std::istream_iterator<std::string> begin(iss), end;

			//putting all the phrases (tokens) in the vector
			std::vector<std::string> phrase(begin, end);

			if (phrase[0].substr(0, 5) == "chars")
			{
				continue;
			}

			if (phrase[0].substr(0, 6) == "common")
			{
				for (int i = 1; i < 5; i++)
				{
					std::istringstream value(phrase[i]);
					std::string discard;
					std::getline(value, discard, '=');

					if (phrase[i].substr(0, 6) == "scaleW")
					{
						int IDValue;
						value >> IDValue;
						primaryFont.w = IDValue;
					}

					if (phrase[i].substr(0, 6) == "scaleH")
					{
						int IDValue;
						value >> IDValue;
						primaryFont.h = IDValue;
					}

					if (phrase[i].substr(0, 4) == "base")
					{
						int IDValue;
						value >> IDValue;
						primaryFont.base = IDValue;
					}

					if (phrase[i].substr(0, 10) == "lineHeight")
					{
						int IDValue;
						value >> IDValue;
						primaryFont.lineHeight = IDValue;
					}
					continue;
				}
				continue;
			}

			if (phrase[0].substr(0, 4) == "info")
			{
				for (int i = 1; i < 2; i++)
				{
					std::istringstream value(phrase[i]);
					std::string discard;
					std::getline(value, discard, '=');

					if (phrase[i].substr(0, 4) == "size")
					{
						int IDValue;
						value >> IDValue;
						primaryFont.size = IDValue;
					}
				}
				continue;
			}

			if (phrase[0].substr(0, 4) == "page")
			{
				std::istringstream value(phrase[2]);
				std::string discard;
				std::getline(value, discard, '=');

				if (phrase[2].substr(0, 4) == "file")
				{
					std::string fname;
					std::getline(value, fname, ' ');
					strncpy(primaryFont.texture, fname.c_str(), 16);
				}
				continue;
			}

			if (phrase[0].substr(0, 4) == "char")
			{
				GLchar ID;
				glyph currentGlyph;
				for (int i = 1; i < 9; i++)
				{
					std::istringstream value(phrase[i]);
					std::string discard;
					std::getline(value, discard, '=');

					if (phrase[i].substr(0, 2) == "id")
					{
						int IDValue;
						value >> IDValue;
						ID = IDValue;
						continue;
					}
					if (phrase[i].substr(0, 7) == "xoffset")
					{
						value >> currentGlyph.xoff;
						continue;
					}
					if (phrase[i].substr(0, 7) == "yoffset")
					{
						value >> currentGlyph.yoff;
						continue;
					}
					if (phrase[i].substr(0, 8) == "xadvance")
					{
						value >> currentGlyph.advance;
						continue;
					}
					if (phrase[i].substr(0, 1) == "x")
					{
						value >> currentGlyph.x;
						continue;
					}
					if (phrase[i].substr(0, 1) == "y")
					{
						value >> currentGlyph.y;
						continue;
					}
					if (phrase[i].substr(0, 5) == "width")
					{
						value >> currentGlyph.w;
						continue;
					}
					if (phrase[i].substr(0, 6) == "height")
					{
						value >> currentGlyph.h;
						continue;
					}
				}
				primaryFontGlyphInfo[ID] = currentGlyph;
				continue;
			}
		}
	}
	else 
	{
		printf("[ERROR] Can't load %s \n", "outpost731/fonts/terminal.fnt");
		// getchar();
	}

	gl_tex_primaryFont = loadBMP_custom("outpost731/fonts/" + primaryFont.texture);
}

void text::renderText(std::string text, GLuint x, GLuint y, float size, glm::vec3 color)
{
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);

	glm::vec2 vertexData[text.length * 6];
	glm::vec2 uvData[text.length * 6];

	int cursorX = x;
	float scale = size / primaryFont.size;

	for (int i = 0; i < text.length; i++)
	{
		glyph cGlyph = primaryFontGlyphInfo[text[i]];
		
		glm::vec2 fourCorners_vert[4];
		glm::vec2 fourCorners_uv[4];

		// Top Left
		fourCorners_vert[0].x = cursorX + (cGlyph.offset.x*scale);
		fourCorners_vert[0].y = y + (cGlyph.offset.y*scale);
		fourCorners_uv[0].x = cGlyph.pos.x;
		fourCorners_uv[0].y = cGlyph.pos.y;
		// Top Right
		fourCorners_vert[1].x = cursorX + ((cGlyph.offset.x + cGlyph.size.x)*scale);
		fourCorners_vert[1].y = y + (cGlyph.offset.y*scale);
		fourCorners_uv[0].x = cGlyph.pos.x + cGlyph.size.x;
		fourCorners_uv[0].y = cGlyph.pos.y;
		// Bottom Left
		fourCorners_vert[2].x = cursorX + (cGlyph.offset.x*scale);
		fourCorners_vert[2].y = y + ((cGlyph.offset.y - cGlyph.size.y)*scale);
		fourCorners_uv[0].x = cGlyph.pos.x;
		fourCorners_uv[0].y = cGlyph.pos.y + cGlyph.size.y;
		// Bottom Right
		fourCorners_vert[3].x = cursorX + ((cGlyph.offset.x + cGlyph.size.x)*scale);
		fourCorners_vert[3].y = y + ((cGlyph.offset.y - cGlyph.size.y)*scale);
		fourCorners_uv[0].x = cGlyph.pos.x + cGlyph.size.x;
		fourCorners_uv[0].y = cGlyph.pos.y + cGlyph.size.y;

		// Order for triangle one is: topLeft, bottomLeft, bottomRight
		vertexData[i * 6 + 0] = fourCorners_vert[0];
		vertexData[i * 6 + 1] = fourCorners_vert[2];
		vertexData[i * 6 + 2] = fourCorners_vert[3];
		uvData[i * 6 + 0] = fourCorners_uv[0];
		uvData[i * 6 + 1] = fourCorners_uv[2];
		uvData[i * 6 + 2] = fourCorners_uv[3];

		// Order for triangle two is: bottomRight, topRight, topLeft
		vertexData[i * 6 + 3] = fourCorners_vert[3];
		vertexData[i * 6 + 4] = fourCorners_vert[1];
		vertexData[i * 6 + 5] = fourCorners_vert[0];
		uvData[i * 6 + 3] = fourCorners_uv[3];
		uvData[i * 6 + 4] = fourCorners_uv[1];
		uvData[i * 6 + 5] = fourCorners_uv[0];

		// Advance cursor for next character
		cursorX += cGlyph.advance*scale;
	}

	// Activate corresponding render state	
	glUseProgram(shd_font_singlecolor);
	glUniform3f(glGetUniformLocation(shd_font_singlecolor, "in_color"), color.x, color.y, color.z);
	glUniform2f(glGetUniformLocation(shd_font_singlecolor, "in_texDimensions"), primaryFont.w, primaryFont.h);
	GLuint MatrixID = glGetUniformLocation(shd_font_singlecolor, "MATRIX_WORLD_VIEW_PROJECTION");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(gl_vao_primaryFont);

	//Enable depth test
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shd_font_singlecolor);



	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, size); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		
		
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
