#ifndef LIMBICEDITOR_MATERIALS_H
#define LIMBICEDITOR_MATERIALS_H

#include <vector>
#include <string>

#define STATUS_SUCCESS 0
#define STATUS_FAILED 1
#define STATUS_FAILEDLOAD 2
#define STATUS_FAILEDWRITE 3
#define STATUS_FAILEDCLOSE 4

struct dlmfheader_t
{
    int nVersion;            // The version number, current version is 0
    char nFlags;            // Flipping the first bit to true means the textures and shaders are stored within the file. Flipping the second bit means textures are stored in KTX instead of DDS
    int nTextures;          // Number of textures put into the shader (per frame of animation)
    int nFrames;            // Defines the number of frames
    int nShaderLength;      // Length of the filename of the shader
    int nTexturesLength;     // Length of the texture filename list
};

class Material
{
public:
    Material();
    int load(char *fname);
    int save(char *fname);
    dlmfheader_t info;
    std::string shader;
    std::vector<std::vector<std::string>> textures;
};

#endif //LIMBICEDITOR_MATERIALS_H
