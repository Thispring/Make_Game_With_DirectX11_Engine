#include "pch.h"
#include "ELight2DUI.h"


ELight2DUI::ELight2DUI()
	: EComponentUI(COMPONENT_TYPE::LIGHT2D, "Light2D")
{
}

ELight2DUI::~ELight2DUI()
{
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
};

void ELight2DUI::Tick_UI()
{
    //====================
    // 빛의 정보를 바꾸는 UI
    //====================
    ImGui::Text("Light2DUI");

    if (ImGui::CollapsingHeader("Light2D", ImGuiTreeNodeFlags_DefaultOpen))
    {
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
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            if (ImGui::ColorEdit3("color 1", col1))
            {
                GetTarget()->Light2D()->SetLightColor(Vec3(col1));
            }
        }

        // Light Dir
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightDir");
        ImGui::SameLine(150);
        Vec3 vDir = GetTarget()->Light2D()->GetLightDir();

        // Light Radius
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightRadius");
        ImGui::SameLine(150);
        float radius = GetTarget()->Light2D()->GetRadius();
        if (ImGui::DragFloat("##LightRadius", &radius))
        {
            GetTarget()->Light2D()->SetRadius(radius);
        }

        // Light Angle
        ImGui::SetNextItemWidth(200);
        ImGui::Text("LightAngle");
        ImGui::SameLine(150);
        float angle = GetTarget()->Light2D()->GetAngle();
        if (ImGui::DragFloat("##LightAngle", &angle))
        {
            GetTarget()->Light2D()->SetAngle(angle);
        }
    }

    ImGui::Dummy(Vec2(0.0f, 1000.0f));


	/*
    ImGui::Text("Rotation");
    ImGui::SameLine(150);
    Vec3 vDegree = vRot * 180 / XM_PI;
    if (ImGui::DragFloat3("##ROTATION", vDegree))
    {
        vRot = vDegree * (XM_PI / 180.f);
        GetTarget()->Transform()->SetRelativeRot(vRot);
    }

    Vec3 GetLightDir() { return m_Info.LightDir; }
    void SetLightDir(Vec3 _Dir) { m_Info.LightDir = _Dir; }

    Vec3 GetAmbient() { return m_Info.Ambient; }
    void SetAmbient(Vec3 _Amb) { m_Info.Ambient = _Amb; }

    */
}