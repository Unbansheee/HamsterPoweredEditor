#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <freetype/freetype.h>
#include <glm/vec2.hpp>

#include "ResourceManagement/Texture.h"

class Font
{
    struct FontChar
    {
        Texture* texture;
        glm::ivec2 Bearing;
        uint32_t Advance = 0;
    };
    
public:
    static Font* LoadFont(const std::string& path, int size);
    static inline std::unordered_map<std::string, Font*> fonts;

    inline FontChar GetChar(char c) { return m_FontChars[c]; };
    std::string GetName() { return m_fontName; };
    std::string GetPath() { return m_path; };
private:
    friend class TextLabel;
    
    Font(const std::string& path, int size);
    ~Font();
    
    std::string m_path;
    int m_size;
    std::map<char, FontChar> m_FontChars;
    
    FT_Face m_face;
    FT_Library m_ft;
    std::string m_fontName;
    static const int FontCharacterLimit = 128;
    

    
};
