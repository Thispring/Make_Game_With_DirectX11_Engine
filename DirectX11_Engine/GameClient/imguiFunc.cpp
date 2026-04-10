// imguiFunc.cpp
// Implementation of the colored button utility declared in imguiFunc.h
// This version avoids using std::min/std::max/std::clamp to prevent conflicts
// with Windows' min/max macros. It defines a small clampf helper local to this file.
#include "pch.h"
#include "imguiFunc.h"
#include <cmath>    // optional (not strictly required here, kept for completeness)

namespace
{
    // Local clamp helper that does not depend on std::min/std::max (avoids Windows macro issues)
    inline float clampf(float v, float lo = 0.0f, float hi = 1.0f)
    {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    }

    inline ImVec4 ClampColor(const ImVec4& c)
    {
        return ImVec4(
            clampf(c.x, 0.0f, 1.0f),
            clampf(c.y, 0.0f, 1.0f),
            clampf(c.z, 0.0f, 1.0f),
            clampf(c.w, 0.0f, 1.0f)
        );
    }

    inline ImVec4 AdjustBrightness(const ImVec4& c, float factor)
    {
        ImVec4 out = c;
        out.x = out.x * factor;
        out.y = out.y * factor;
        out.z = out.z * factor;
        // preserve alpha
        return ClampColor(out);
    }

    inline float Luminance(const ImVec4& c)
    {
        // Perceptual luminance (Rec.709)
        return 0.2126f * c.x + 0.7152f * c.y + 0.0722f * c.z;
    }
} // anonymous namespace

namespace ImGuiFunc
{
    ImGuiKey KeyToImGuiKey(KEY _key)
    {
        int idx = (int)_key;
        if (idx >= (int)KEY::A && idx <= (int)KEY::Z)
            return static_cast<ImGuiKey>(ImGuiKey_A + idx - (int)KEY::A);
        return ImGuiKey_None;
    }

    bool ColoredButton(const char* label, const ImVec4& baseColor, const ImVec2& size)
    {
        // Derive hovered/active by simple brightness multipliers
        const float hoverFactor = 0.92f;
        const float activeFactor = 0.82f;

        ImVec4 col_base = ClampColor(baseColor);
        ImVec4 col_hover = AdjustBrightness(col_base, hoverFactor);
        ImVec4 col_active = AdjustBrightness(col_base, activeFactor);

        // Choose readable text color (white for dark backgrounds, black otherwise)
        ImVec4 text_col = (Luminance(col_base) < 0.5f) ? ImVec4(1.f, 1.f, 1.f, 1.f) : ImVec4(0.f, 0.f, 0.f, 1.f);

        // Push style colors for Button, Hovered, Active and Text
        ImGui::PushStyleColor(ImGuiCol_Button, col_base);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);
        ImGui::PushStyleColor(ImGuiCol_Text, text_col);

        bool clicked = ImGui::Button(label, size);

        ImGui::PopStyleColor(4);
        return clicked;
    }
} // namespace ImGuiFunc