#include "Material.h"

Material::Material()
{
    info.nVersion = 0;
    info.nFlags = 0;
    info.nTextures = 1;
    info.nFrames = 1;
    info.nShaderLength = 0;
    info.nTexturesLength = 0;

    shader = "default.shd";
    std::vector<std::string> firstFrame;
    std::string untitledTexture = "untitled.dds";
    firstFrame.push_back(untitledTexture);
    textures.push_back(firstFrame);

}
int Material::load(char *fname)
{
    int result = STATUS_SUCCESS;
    FILE* lmfFile;

#ifdef WINDOWS
    if (fopen_s(&lmatFile, fname, "rb"))
#else
    lmfFile = fopen(fname, "rb");
    if (lmfFile == NULL)
#endif
    {
        fprintf(stderr, "[ERROR] BSP failed to open!\n");
        result = STATUS_FAILEDLOAD;
    }

    // Load the header into memory so we can analyze it
    fread(&info, sizeof(dlmfheader_t), 1, lmfFile);

    // Load shader name
    char temp[info.nShaderLength];
    fread(&temp, info.nShaderLength, 1, lmfFile);
    shader = temp;

    // Load texture names
    char tempTextures[info.nTexturesLength];
    fread(&tempTextures, info.nTexturesLength, 1, lmfFile);
    std::string texNames = tempTextures;

    // Extract shader names into their positions in the vector array

    if (fclose(lmfFile))
    {
        fprintf(stderr, "[ERROR] BSP failed to close!\n");
        result = STATUS_FAILEDCLOSE;
    }

    return result;
}

int Material::save(char *fname)
{
    return STATUS_SUCCESS;
}