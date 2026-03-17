#include "pch.h"
#include "EComponentUI.h"

#include "imguiFunc.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "EditorMgr.h"

EComponentUI::EComponentUI(COMPONENT_TYPE _Type, const string& _Name)
	: EditorUI(_Name)
	, m_ComType(_Type)
{
}

EComponentUI::~EComponentUI()
{
}

void EComponentUI::SetTarget(Ptr<GameObject> _Object)
{
	m_Target = _Object;

	if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_ComType))
		SetActive(false);
	else
		SetActive(true);
}

void EComponentUI::OutputTitle(const string& _Title)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::Button(_Title.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}

void EComponentUI::OutputTitle(const string& _Title, ImVec4 _Color)
{
	ImGui::PushID(0);
	// RGB 값을 모두 최대로 설정 (255, 255, 255)
	ImGui::PushStyleColor(ImGuiCol_Button, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _Color);
	ImGui::Button(_Title.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}

bool EComponentUI::DeleteComponent(COMPONENT_TYPE _Type)
{
	// Inspector 에서만 Delete UI를 표시하도록 부모 UI를 검사
	Ptr<EditorUI> pParent = GetParentUI();
	if (pParent == nullptr)
		return false;

	// 부모 UI의 이름이 "Inspector"일 때만 삭제 UI 노출
	if (pParent->GetUIName() != "Inspector")
		return false;


	// Target이 들고있는 Component 제거,
	// 자식 UI 클래스의 COMPONENT_TYPE을 파라미터로 전달하여, 해당 Component를 제거
	ImGui::SameLine(250.f);

	if (ImGuiFunc::ColoredButton("Delete\nThis Component",
		ColorConvertIntToVec4(125, 23, 20), ImVec2(120.f, 40.f)))
	{
		ImGui::OpenPopup("DeleteThisComponent?");
	}

	if (ImGui::BeginPopupModal("DeleteThisComponent?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want delete this component?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			GetTarget()->ReleaseComponent(_Type);

			// 현재 Level에 변경점을 알림
			// LevelMgr의 ChangeLevel는 private 함수
			Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
			ChangeLevel(pLevel->GetKey());
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			return true;
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}
	SPACING_UI(5);
}
