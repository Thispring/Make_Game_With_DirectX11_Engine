#include "pch.h"
#include "ELight2DUI.h"


ELight2DUI::ELight2DUI()
	: EComponentUI(COMPONENT_TYPE::LIGHT2D, "Light2D")
{
}

ELight2DUI::~ELight2DUI()
{
}


void ELight2DUI::Tick_UI()
{
    //====================
    // 빛의 정보를 바꾸는 UI
    //====================
    OutputTitle("Light2DUI");

    // 컴포넌트 제거 버튼
    DeleteComponent(COMPONENT_TYPE::LIGHT2D);

    if (ImGui::CollapsingHeader("Light2D", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // nullptr 체크
        if (GetTarget()->Light2D() == nullptr)
            return;


        // Light Type
        ImGui::Text("LightType");
        ImGui::SameLine();
        LIGHT_TYPE LightType = GetTarget()->Light2D()->GetLightType();
        const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
        ImGui::SetNextItemWidth(200);
        // Light Type은 Enum이므로 정수로 변환 가능
        if (ImGui::Combo("##LightType", (int*)&LightType, items, 3))
        {
            GetTarget()->Light2D()->SetLightType(LightType);
        }


        // Light Color
        ImGui::SetNextItemWidth(200);
        Vec3 vColor = GetTarget()->Light2D()->GetLightColor();
        ImGui::SeparatorText("LightColor/Pickers");
        {
            if (ImGui::ColorEdit3("color 1", vColor))
            {
                GetTarget()->Light2D()->SetLightColor(vColor);
            }
        }


        // Light Dir
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightDir");
        ImGui::SameLine(150);
        Vec3 vDir = GetTarget()->Light2D()->GetLightDir();
        if (ImGui::DragFloat3("##LightDir", vDir))
        {
            GetTarget()->Light2D()->SetLightDir(vDir);
        }


        // Light Ambient
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightAmbient");
        ImGui::SameLine(150);
        Vec3 vAmb = GetTarget()->Light2D()->GetAmbient();
        if (ImGui::DragFloat3("##LightAmbient", vAmb))
        {
            GetTarget()->Light2D()->SetAmbient(vAmb);
        }
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Only available for Directional Light.");
        SPACING_UI(3);

        // Light Radius
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightRadius");
        ImGui::SameLine(150);
        float radius = GetTarget()->Light2D()->GetRadius();
        if (ImGui::DragFloat("##LightRadius", &radius))
        {
            GetTarget()->Light2D()->SetRadius(radius);
        }
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Only available for Point, Spot Light.");
        SPACING_UI(3);

        // Light Angle
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightAngle");
        ImGui::SameLine(150);
        float angle = GetTarget()->Light2D()->GetAngle();
        if (ImGui::DragFloat("##LightAngle", &angle))
        {
            GetTarget()->Light2D()->SetAngle(angle);
        }
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Only available for Spot Light.");
        SPACING_UI(3);
    }

    ImGui::Dummy(Vec2(0.0f, 1000.0f));

}