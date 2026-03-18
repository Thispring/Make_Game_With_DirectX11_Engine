#include "pch.h"
#include "FlipbookMaker.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
// ImGui::InputText의 기본 정의는 char* 버퍼, 이를 해결하기 위한 헤더
//#include "imgui/imgui_stdlib.h"
#include "imguiFunc.h"

FlipbookMaker::FlipbookMaker()
	: EditorUI("FlipbookMaker")

	, m_SpriteName{}
	, m_FlipbookName{}
	, m_SpriteCount(0)
{
}

FlipbookMaker::~FlipbookMaker()
{
}

void FlipbookMaker::ClearSetting()
{
	m_SpriteName = {};
	m_FlipbookName = {};
	m_SpriteCount = 0;
}

void FlipbookMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making Flipbook", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine(300.f);

	#pragma region SaveFlipbookBtn
	if (ImGuiFunc::ColoredButton("SaveFlipbook##FlipbookSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("FlipbookSave?");
	}

	// 모달 창을 매 프레임 마다 호출되게 하고,
	// 팝업 상태가 Open일 때, 실행된다.
	// 
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("FlipbookSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 및 초기화
			AssetMgr::GetInst()->CreateEngineFlipbook(m_SpriteName, m_FlipbookName, m_SpriteCount);
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

	#pragma region Sprite Name
	OutputTitle("Sprite Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string spriteName = string(m_SpriteName.begin(), m_SpriteName.end());
	if (ImGui::InputTextWithHint("##SPRITENAME", "Example: Sprite\\cat_Idle.sprite", &spriteName))
	{
		wstring spName = wstring(spriteName.begin(), spriteName.end());
		SetSpriteName(spName);
	}
	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			// DragDrop으로 가져온 경우, 파일경로와 확장자, _%d을 제거
			wstring assetName = pAsset->GetKey();

			// 가져온 Texture의 Key 문자열을 세팅
			SetSpriteName(ExtractFileName(assetName));
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Please enter the name of the Sprite you want to create as a Flipbook.\nFor Sprites with the same name but different numbers in the list\n(e.g., test_Idle_0.sprite, test_Idle_1.sprite), just enter 'test_Idle'");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region FlipbookName
	OutputTitle("Flipbook name to save", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string fbName = string(m_FlipbookName.begin(), m_FlipbookName.end());
	if (ImGui::InputTextWithHint("##FLIPBOOKNAMETOSAVE", "Name your Flipbook", &fbName))
	{
		wstring wfbName = wstring(fbName.begin(), fbName.end());
		SetFlipbookName(wfbName);
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region SpriteCount
	OutputTitle("SpriteCount", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##SPRITECOUNT", &m_SpriteCount, 1.f, 0, INT_MAX))
	{
		SetSpriteCount(m_SpriteCount);
	}
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Number of sprites for the flipbook");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

}
