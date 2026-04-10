// imguiFunc.h
// Utility to create a colored ImGui button with automatically derived hover/active/text colors.
//
// Usage:
//   #include "imguiFunc.h"
//   if (ImGuiFunc::ColoredButton("Destroy", ImVec4(1.f,0.2f,0.2f,1.f), ImVec2(100.f,20.f))) { /* clicked */ }

#pragma once
#include "imgui\imgui.h"
#include "KeyMgr.h"

namespace ImGuiFunc
{
    // Primary API (declared, implemented in imguiFunc.cpp)
    bool ColoredButton(const char* label, const ImVec4& baseColor, const ImVec2& size = ImVec2(0, 0));

    // KEY::A~Z 를 대응하는 ImGuiKey 로 변환 (범위 밖이면 ImGuiKey_None 반환)
    ImGuiKey KeyToImGuiKey(KEY _key);

    // Convenience overload (implemented inline to forward to primary API)
    inline bool ColoredButton(const char* label, const ImVec4& baseColor, float w, float h)
    {
        return ColoredButton(label, baseColor, ImVec2(w, h));
    }

    // Template overload for user vector types (expects .x/.y/.z/.w and .x/.y members)
    template<typename Vec4T, typename Vec2T>
    inline bool ColoredButton(const char* label, const Vec4T& baseColor, const Vec2T& size)
    {
        return ColoredButton(label, ImVec4(baseColor.x, baseColor.y, baseColor.z, baseColor.w), ImVec2(size.x, size.y));
    }
} // namespace ImGuiFunc