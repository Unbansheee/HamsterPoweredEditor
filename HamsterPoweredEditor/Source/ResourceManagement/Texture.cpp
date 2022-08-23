#include "Texture.h"

#include <iostream>

#include "Rendering/Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    : rendererID(0), filePath(path), localBuffer(NULL), width(0), height(0), bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);
    if (localBuffer == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    GLenum internalFormat = 0, dataFormat = 0;
    if (bpp == 1)
    {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }
    else if (bpp == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else if (bpp == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else
    {
        std::cout << "Image format not supported" << std::endl;
    }
    
    glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
    glTextureStorage2D(rendererID, 1, internalFormat, width, height);

    glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, localBuffer);

    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned slot) const
{
    glActiveTexture(slot);
    glBindTextureUnit(slot, rendererID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
