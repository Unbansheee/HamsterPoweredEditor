#pragma once
#include "imgui.h"
#include "Core/Font.h"
#include "ResourceManagement/Texture.h"
#include "imgui/imgui_stdlib.h"

class Font;

namespace ImGui
{
    inline void Image(std::shared_ptr<Texture> texture, const ImVec2& size, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        Image((ImTextureID)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);
    }

    inline void Image(Texture* texture, const ImVec2& size, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        Image((ImTextureID)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);
    }

    inline void ImageScaledH(Texture* texture, float scaleFactor = 1.f, bool clampSize = false, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        ImVec2 size = {ImGui::GetContentRegionAvail().x*scaleFactor, ImGui::GetContentRegionAvail().x*scaleFactor / (float)texture->GetWidth() * (float)texture->GetHeight()};

        if (clampSize)
        {
            size.x = std::min(size.x, (float)texture->GetWidth());
            size.y = std::min(size.y, (float)texture->GetHeight());
        }

        
        Image((void*)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);
    }

    inline bool FontSelector(const char* label, Font** font)
    {
        bool edited = true;
        if (ImGui::BeginCombo("Font", (*font)->GetName().c_str()))
        {
            for (auto it = Font::fonts.begin(); it != Font::fonts.end(); it++)
            {
                bool is_selected = (it->first.first == (*font)->GetPath());
                if (ImGui::Selectable(it->second->GetName().c_str(), is_selected))
                {
                    (*font) = it->second;
                    edited = true;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        return edited;
    }
}
