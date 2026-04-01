#include "pch.h"
#include "ECameraUI.h"
#include "LevelMgr.h"

ECameraUI::ECameraUI()
	: EComponentUI(COMPONENT_TYPE::CAMERA, "Camera")
{
}

ECameraUI::~ECameraUI()
{
}

void ECameraUI::Tick_UI()
{
    OutputTitle("CameraUI");

    // 컴포넌트 제거 버튼
    DeleteComponent(COMPONENT_TYPE::CAMERA);

    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // nullptr 체크
        if (GetTarget()->Camera() == nullptr)
            return;

        int SameLine = 180;

        // ==========
        // LayerCheck
        // ==========
        if (ImGui::TreeNode("LayerCheck"))
        {
            UINT LayerCheck = GetTarget()->Camera()->GetLayerCheck();
            Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();

            string strLayerName[MAX_LAYER] = {};
            bool selected[MAX_LAYER] = {};

            for (UINT i = 0; i < MAX_LAYER; ++i)
            {
                wstring Name = pCurLevel->GetLayer(i)->GetName();
                strLayerName[i] = string(Name.begin(), Name.end());

                if (strLayerName[i].empty())
                {
                    char buff[255] = {};
                    sprintf_s(buff, "None##%d", i);
                    strLayerName[i] = buff;
                }

                selected[i] = LayerCheck & (1 << i);
            }

            if (ImGui::BeginTable("##LayerCheck", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < 32; i++)
                {
                    ImGui::TableNextColumn();

                    if (ImGui::Selectable(strLayerName[i].c_str(), &selected[i]))
                    {
                        GetTarget()->Camera()->LayerCheck(i);
                    }
                }
                ImGui::EndTable();
            }

            SetSizeAsChild(Vec2(0.f, 1100.f));

            ImGui::TreePop();
        }
        else
        {
            SetSizeAsChild(Vec2(0.f, 300.f));
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // ==============
        // ProjectionType
        // ==============
        ImGui::Text("ProjectionType");
        ImGui::SameLine(SameLine);

        PROJ_TYPE ProjType = GetTarget()->Camera()->GetProjType();
        const char* items[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };
        ImGui::SetNextItemWidth(200);
        if (ImGui::Combo("##ProjType", (int*)&ProjType, items, 2))
        {
            GetTarget()->Camera()->SetProjType(ProjType);
        }

        // ===
        // FOV
        // ===
        ImGui::BeginDisabled(ProjType == PROJ_TYPE::ORTHOGRAPHIC);

        ImGui::Text("FOV");
        ImGui::SameLine(SameLine);
        float FOV = GetTarget()->Camera()->GetFOV();
        ImGui::SetNextItemWidth(200);
        if (ImGui::DragFloat("##FOV", &FOV, 0.1f))
        {
            GetTarget()->Camera()->SetFOV(FOV);
        }

        ImGui::EndDisabled();


        //====
        // Far
        //====
        ImGui::Text("Far");
        ImGui::SameLine(SameLine);
        float Far = GetTarget()->Camera()->GetFar();
        ImGui::SetNextItemWidth(200);
        if (ImGui::DragFloat("##Far", &Far, 0.1f))
        {
            GetTarget()->Camera()->SetFar(Far);
        }


        //======
        // Width
        //======
        ImGui::Text("Width");
        ImGui::SameLine(SameLine);
        float Width = GetTarget()->Camera()->GetWidth();
        ImGui::SetNextItemWidth(200);
        if (ImGui::DragFloat("##Width", &Width, 0.1f))
        {
            GetTarget()->Camera()->SetWidth(Width);
        }
        

        //=============
        // AspectRatio
        //=============
        ImGui::Text("AspectRatio");
        ImGui::SameLine(SameLine);
        float AspectRatio = GetTarget()->Camera()->GetAspectRatio();
        ImGui::SetNextItemWidth(200);
        if (ImGui::DragFloat("##AspectRatio", &AspectRatio, 0.1f))
        {
            GetTarget()->Camera()->SetAspectRatio(AspectRatio);
        }


        //===========
        // OrthoScale
        //===========
        ImGui::Text("OrthoScale");
        ImGui::SameLine(SameLine);
        float OrthoScale = GetTarget()->Camera()->GetOrthoScale();
        ImGui::SetNextItemWidth(200);
        if (ImGui::DragFloat("##OrthoScale", &OrthoScale, 0.1f))
        {
            GetTarget()->Camera()->SetOrthoScale(OrthoScale);
        }


        //========
        // IsUICam
        //========
        ImGui::Text("IsUICam");      
        ImGui::SameLine(SameLine);
        bool IsUICam = GetTarget()->Camera()->GetIsUICam();
        ImGui::SetNextItemWidth(200);
        if (ImGui::Checkbox("##IsUICam", &IsUICam))
        {
            GetTarget()->Camera()->SetIsUICam(IsUICam);
        }

    }
}

