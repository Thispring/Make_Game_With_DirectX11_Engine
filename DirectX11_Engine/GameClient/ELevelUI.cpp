#include "pch.h"
#include "ELevelUI.h"

#include "AssetMgr.h"
#include "LevelMgr.h"
#include "EditorMgr.h"

ELevelUI::ELevelUI()
	: EAssetUI(ASSET_TYPE::LEVEL)
    , m_LayerNames{}
{
}

ELevelUI::~ELevelUI()
{
}

void ELevelUI::Tick_UI()
{
	OutputTitle();
    SPACING_UI(2);

	Ptr<ALevel> pLevel = (ALevel*)GetTargetAsset().Get();
	Layer* pLayer = pLevel->GetLayer();
    UINT* pMatrix = pLevel->GetCollisionMatrix();
	
    //ImGuiTreeNodeFlags_DefaultOpen
    #pragma region Layer Name Setting
    if (ImGui::CollapsingHeader("Layer List"))
    {
        m_LayerNames.clear();
	    for (int i = 0; i < MAX_LAYER; ++i)
	    {
            string layerName = string(pLayer[i].GetName().begin(), pLayer[i].GetName().end());
            m_LayerNames.push_back(layerName);

		    // 설정한 이름 문자열과, Layer 번호 (i) 출력
		    layerName = to_string(i) + "         ";
		    layerName += string(pLayer[i].GetName().begin(), pLayer[i].GetName().end());
		    char buffer[255];
		    snprintf(buffer, sizeof(buffer), "##LAYER_NAME%d", i);
		    const char* Lable = buffer;
		    if (ImGui::InputText(Lable, &layerName))
		    {
                // to_string함수는 객체없이 단독으로 사용하면, 객체가 제거된 채로 반환
                string numStr = to_string(i);

                // 가독성을 위해 추가한 숫자와 공백을 제거하고 전달
                //layerName.erase((layerName.find(numStr) != std::string::npos));
                size_t pos;
                while ((pos = layerName.find(numStr)) != std::string::npos) 
                {
                    layerName.erase(pos, numStr.length());
                }
                // 공백 모두 삭제
                layerName.erase(remove(layerName.begin(), layerName.end(), ' '), layerName.end());
                
                wstring wLayerName = wstring(layerName.begin(), layerName.end());
			    // 이름이 없는 Layer 이름 설정할 수 있게 구현
			    pLayer[i].SetName(wLayerName);
		    }

	    }
	    ImGui::Spacing();
    }
    SPACING_UI(2);
    ImGui::Separator();
    #pragma endregion


    #pragma region Level Collision Setting
    // 충돌 체크 등록은
    // Level의
    // CheckCollisionLayer 함수로 진행

    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    if (ImGui::CollapsingHeader("CollisionLayer Setting", ImGuiTreeNodeFlags_DefaultOpen))
    {
        m_LayerNames.clear();
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            string layerName = string(pLayer[i].GetName().begin(), pLayer[i].GetName().end());
            m_LayerNames.push_back(layerName);
        }

        if (ImGui::TreeNode("CollisionLayer Table"))
        {
            const char* column_names[] = { "Track", "cabasa", "ride", "smash", "tom-hi", "tom-mid", "tom-low", "hihat-o", "hihat-c", "snare-s", "snare-c", "clap", "rim", "kick" };
            const int columns_count = MAX_LAYER;
            const int rows_count = MAX_LAYER;

            static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;
            static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
            static bool bools[MAX_LAYER * MAX_LAYER] = {}; // Dummy storage selection storage
            static int frozen_cols = 1;
            static int frozen_rows = 2;
            //ImGui::CheckboxFlags("_ScrollX", &table_flags, ImGuiTableFlags_ScrollX);
            //ImGui::CheckboxFlags("_ScrollY", &table_flags, ImGuiTableFlags_ScrollY);
            //ImGui::CheckboxFlags("_Resizable", &table_flags, ImGuiTableFlags_Resizable);
            //ImGui::CheckboxFlags("_Sortable", &table_flags, ImGuiTableFlags_Sortable);
            //ImGui::CheckboxFlags("_NoBordersInBody", &table_flags, ImGuiTableFlags_NoBordersInBody);
            //ImGui::CheckboxFlags("_HighlightHoveredColumn", &table_flags, ImGuiTableFlags_HighlightHoveredColumn);
            //ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            //ImGui::SliderInt("Frozen columns", &frozen_cols, 0, 2);
            //ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            //ImGui::SliderInt("Frozen rows", &frozen_rows, 0, 2);
            //ImGui::CheckboxFlags("Disable header contributing to column width", &column_flags, ImGuiTableColumnFlags_NoHeaderWidth);

            //if (ImGui::TreeNode("Style settings"))
            //{
            //    ImGui::SameLine();
            //    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            //    ImGui::SliderAngle("style.TableAngledHeadersAngle", &ImGui::GetStyle().TableAngledHeadersAngle, -50.0f, +50.0f);
            //    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            //    ImGui::SliderFloat2("style.TableAngledHeadersTextAlign", (float*)&ImGui::GetStyle().TableAngledHeadersTextAlign, 0.0f, 1.0f, "%.2f");
            //    ImGui::TreePop();
            //}

            if (ImGui::BeginTable("table_angled_headers", columns_count, table_flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 40)))
            {
                ImGui::TableSetupColumn("Layer Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
                for (int n = 1; n < columns_count; n++)
                    ImGui::TableSetupColumn(m_LayerNames[n].c_str(), column_flags);
                ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

                ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
                ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
                for (int row = 0; row < rows_count; row++)
                {
                    ImGui::PushID(row);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text(m_LayerNames[row].c_str());
                    for (int column = 1; column < columns_count; column++)
                        if (ImGui::TableSetColumnIndex(column))
                        {
                            ImGui::PushID(column);

                            // 매핑: 작은 인덱스를 row로, 큰 인덱스를 bit 위치(col)로 사용
                            UINT r = (row < column) ? row : column;
                            UINT c = (row < column) ? column : row;

                            bool enabled = (pMatrix[r] & (1u << c)) != 0u;

                            if (row == column)
                            {
                                // 자기 자신은 비활성화(또는 다른 표시)
                                ImGui::BeginDisabled();
                                ImGui::Checkbox("", &enabled);
                                ImGui::EndDisabled();
                            }
                            else
                            {
                                // Checkbox 변경 시 현재 구현된 토글 함수 호출
                                if (ImGui::Checkbox("", &enabled))
                                {
                                    pLevel->CheckCollisionLayer((UINT)row, (UINT)column);
                                }
                            }

                            ImGui::PopID();
                        }
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }

    #pragma endregion   

}
