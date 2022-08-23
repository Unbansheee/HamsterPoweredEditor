#pragma once
#include "Rendering/Renderer.h"

class Texture
{
private:

    std::string filePath;
    unsigned char* localBuffer;
    int width, height, bpp;

public:
    unsigned int rendererID;
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    inline int GetWidth() const {return width;}
    inline int GetHeight() const {return height;}
};
