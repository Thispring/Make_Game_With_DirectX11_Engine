#include "pch.h"
#include "LevelMaker.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "imguiFunc.h"


LevelMaker::LevelMaker()
	: EditorUI("LevelMaker")
	, m_LevelName {}
{
}

LevelMaker::~LevelMaker()
{
}


void LevelMaker::ClearSetting()
{
	m_LevelName = {};
}

void LevelMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making Level", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine(300.f);
	#pragma region LevelSaveBtn
	if (ImGuiFunc::ColoredButton("SaveLevel##LevelSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("LevelSave?");
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("LevelSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 및 초기화
			// AssetMgr 호출
			AssetMgr::GetInst()->CreateEngineLevel(m_LevelName);
			ClearSetting();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	SPACING_UI(7);
	ImGui::Separator();
	#pragma endregion

	#pragma region LevelName
	OutputTitle("Level Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string levelName = string(m_LevelName.begin(), m_LevelName.end());
	if (ImGui::InputTextWithHint("##LEVELNAMETOSAVE", "Name your Level", &levelName))
	{
		wstring wlevelName = wstring(levelName.begin(), levelName.end());
		SetLevelName(wlevelName);
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
			"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion
}