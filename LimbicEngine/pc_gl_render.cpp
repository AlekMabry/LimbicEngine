#include "pc_gl_render.hpp"
#include "shaderLoader.hpp"
#include "textureLoader.hpp"

pc_gl_render::pc_gl_render(io* inOutStream, int w, int h, float fov)
{
	screenWidth = w;
	screenHeight = h;
	FOV = fov;
	statusStream = inOutStream;

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "Outpost 731", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	//Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Load shaders and compile
	opaqueShader = LoadShaders("outpost731/shaders/map_default_vert.shader", "outpost731/shaders/map_default_frag.shader");

	// Load textures
	tex_missing = loadBMP_custom("outpost731/textures/missingTexture.bmp");

	// Create the VAOs (Vertex Array Objects)
	glGenVertexArrays(1, &gl_vao);
	glBindVertexArray(gl_vao);
}

int pc_gl_render::openWAD(const char* fname)
{
	if (fopen_s(&wadFile, fname, "rb"))
	{
		statusStream->submitError("WAD failed to open!");
		return STATUS_FAILEDLOAD;
	}

	// Load WAD header
	fread(&wadHeader, sizeof(dwadheader_t), 1, wadFile);

	// Load all of the entries in the directory
	wadEntryArrayLen = wadHeader.nDir;
	wadEntryArray = new dentry_t[wadEntryArrayLen];
	fseek(wadFile, wadHeader.nDirOffset, SEEK_SET);
	for (int i = 0; i < wadEntryArrayLen; i++) {
		fread(&wadEntryArray[i], sizeof(dentry_t), 1, wadFile);
	}

	return STATUS_SUCCESS;
}

int pc_gl_render::addMatWAD(const char* texname, vec2 dimensions, int &id, int type)
{
	id = matArray.size();

	// Load textures into openGL if they are in the WAD file instead
	d_material_opaque cMat;
	glGenTextures(1, &cMat.texture);

	cMat.dimensions = dimensions;

	// First find the texture itself in the WAD entry list
	int texID_wad = -1;
	std::string strA = _strdup(texname);
	for (int c = 0; c < strA.size(); c++)
	{
		strA[c] = toupper(strA[c]);
	}
	for (int j = 0; j < wadEntryArrayLen; j++)
	{
		std::string strB = _strdup(wadEntryArray[j].szName);
		// for (int k = 0; k < 16; k++)
		for (int k = 0; k < strB.size(); k++)
		{
			strB[k] = toupper(strB[k]);
		}

		// Compare
		if (strA == strB)
		{
			texID_wad = j;
		}
	}
	if (texID_wad == -1)
	{
		cMat.texture = tex_missing;
		matArray.push_back(cMat);
		std::cout << "[CRITICAL ERROR] Texture " << texname << " not found! \n";
		return STATUS_FAILED;
	}
	else
	{
		// Now load that texture info
		dtexdata_t wadTexInfo;
		fseek(wadFile, wadEntryArray[texID_wad].nFilePos, SEEK_SET);
		fread(&wadTexInfo, sizeof(dtexdata_t), 1, wadFile);

		int totalPixels = wadTexInfo.nWidth * wadTexInfo.nHeight;
		unsigned char* data = new unsigned char[totalPixels * 3];

		for (int j = 0; j < totalPixels; j++)
		{
			/*
			The texture consists of four mipmap levels, with each pixel represented as a byte
			that points to a 24-bit color in a color pallate

			The offset to the texture is dmiptex_gld_t(Position in WAD) + dmiptex_gld_t.nOffset[0]
			*/
			fseek(wadFile, wadEntryArray[texID_wad].nFilePos + wadTexInfo.nOffsets[0] + j, SEEK_SET);
			unsigned char currentColor;
			fread(&currentColor, sizeof(unsigned char), 1, wadFile);

			// Find what that color actually is by looking it up in the table
			// The table position is at the end of the fourth mipmap after two dummy bytes
			fseek(wadFile, wadEntryArray[texID_wad].nFilePos + wadTexInfo.nOffsets[3] +
				(totalPixels / 64) + 2 + ((int)currentColor * 3), SEEK_SET);
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			fread(&red, sizeof(unsigned char), 1, wadFile);
			fread(&green, sizeof(unsigned char), 1, wadFile);
			fread(&blue, sizeof(unsigned char), 1, wadFile);
			data[(j * 3)] = red;
			data[(j * 3) + 1] = green;
			data[(j * 3) + 2] = blue;
		}

		glBindTexture(GL_TEXTURE_2D, cMat.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wadTexInfo.nWidth,
			wadTexInfo.nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] data;
		matArray.push_back(cMat);
	}
	return STATUS_SUCCESS;
}

int pc_gl_render::closeWAD()
{
	if (wadFile)
	{
		if (fclose(wadFile))
		{
			fprintf(stderr, "[error] WAD failed to close!\n");
			return STATUS_FAILED;
		}
	}

	delete[] wadEntryArray;
	wadEntryArrayLen = 0;

	return STATUS_SUCCESS;
}

int pc_gl_render::pushVertices(int iMat, std::vector<float> &vertData, std::vector<float> &uvData)
{
	GLuint gl_vertData;
	GLuint gl_uvData;
	int size;

	glGenBuffers(1, &gl_vertData);
	glGenBuffers(1, &gl_uvData);

	glBindBuffer(GL_ARRAY_BUFFER, gl_vertData);
	glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), &vertData[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, gl_uvData);
	glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float), &uvData[0], GL_DYNAMIC_DRAW);

	size = vertData.size();

	matArray[iMat].size.push_back(size);
	matArray[iMat].vertData.push_back(gl_vertData);
	matArray[iMat].uvData.push_back(gl_uvData);

	return STATUS_SUCCESS;
}

int pc_gl_render::setWorldView(vec3 pos, vec3 dir, vec3 up, float fov)
{
	world_gl_transform_projection = perspective(radians(fov), (float) screenWidth / (float) screenHeight, (float) 0.1, (float) 2048.0);
	world_gl_transform_view = lookAt(
		pos,				// Camera position in world space
		pos + dir,
		up					//Head is up
	);
	world_gl_transform_world_view = world_gl_transform_projection * world_gl_transform_view;

	return STATUS_SUCCESS;
}

void pc_gl_render::render3D()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(opaqueShader);

	GLuint MatrixID = glGetUniformLocation(opaqueShader, "MATRIX_WORLD_VIEW_PROJECTION");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &world_gl_transform_world_view[0][0]);

	GLuint textureDimensions = glGetUniformLocation(opaqueShader, "in_texDimensions");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	for (int i = 0; i < matArray.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, matArray[i].texture);
		glUniform2f(textureDimensions, matArray[i].dimensions.x, matArray[i].dimensions.y);

		for (int j = 0; j < matArray[i].vertData.size(); j++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, matArray[i].vertData[j]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, matArray[i].uvData[j]);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, matArray[i].size[j]);

			glDeleteBuffers(1, &matArray[i].vertData[j]);
			glDeleteBuffers(1, &matArray[i].uvData[j]);
		}

		matArray[i].vertData.clear();
		matArray[i].uvData.clear();
		matArray[i].size.clear();
	}
}

int pc_gl_render::uploadFramebuffer()
{
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();

	return STATUS_SUCCESS;	// Success
}

GLFWwindow* pc_gl_render::getWindow()
{
	return window;
}