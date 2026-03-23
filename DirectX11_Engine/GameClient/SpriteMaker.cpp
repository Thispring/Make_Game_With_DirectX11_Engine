#include "pch.h"
#include "SpriteMaker.h"

#include "AssetMgr.h"
// ImGui::InputText의 기본 정의는 char* 버퍼, 이를 해결하기 위한 헤더
#include "EditorMgr.h"
#include "imguiFunc.h"

SpriteMaker::SpriteMaker()
	: EditorUI("SpriteMaker")
	, m_TextureName{}
	, m_SliceUV{}
	, m_StartLoop(0)
	, m_EndLoop(0)
	, m_SpriteName{}
	, m_OriIdx(0)
	, m_Row(0)
	, m_Col(0)
	, m_EndOriIdx(0)

{
}

SpriteMaker::~SpriteMaker()
{
}

void SpriteMaker::ClearSetting()
{
	// 모든 멤버의 값을 0으로 초기화 하는 함수
	m_TextureName = {};
	m_SliceUV = Vec2{ 0.f, 0.f };
	m_StartLoop = 0;
	m_EndLoop = 0;
	m_SpriteName = {};
	m_OriIdx = 0;
	m_Row = 0;
	m_Col = 0;
	m_EndOriIdx = 0;
}

void SpriteMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making Sprite", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// 저장 버튼을 위쪽에 두고 싶다면 아래 주석을 풀고, 버튼로직을 Title 아래 배치
	ImGui::SameLine(300.f);
	#pragma region SpriteSaveBtn
	/******************************************************************************************************************************
	* 오버로딩된 함수 호출
	* void AssetMgr::CreateEngineSprite(wstring _Name, Vec2 _Slice, int _StartLoop, int _EndLoop, wstring _Path, float _Origin)
	* Path의 경우 다음과 같이 전달합니다: L"Sprite\\name_Action_"
	******************************************************************************************************************************/
	if (ImGuiFunc::ColoredButton("SaveSprite##SpriteSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("SpriteSave?");
	}
	// 모달 창을 매 프레임 마다 호출되게 하고,
	// 팝업 상태가 Open일 때, 실행된다.
	// 
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("SpriteSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();
		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");
		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 및 초기화
			AssetMgr::GetInst()->CreateEngineSprite(m_TextureName, m_SliceUV, m_StartLoop, m_EndLoop, m_SpriteName, m_OriIdx, m_EndOriIdx, m_Row, m_Col);
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

	#pragma region Texture Name
	OutputTitle("Texture Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string Name = string(m_TextureName.begin(), m_TextureName.end());
	if (ImGui::InputTextWithHint("##TEXTURENAME", "Example: cubeImg", &Name))
	{
		wstring wName = wstring(Name.begin(), Name.end());
		SetTextureName(wName);
	}
	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		/**************************************************************
		* 마우스가 때졌을 때의 조건, 전달한 Key값과 동일한지 확인
		*
		* 동작 의도에 맞게, 어느 ImGui에서 전달한 Key인지를 조건으로 구별하여
		* MeshRenderUI이면 ContentUI의 Mesh 목록에 있는 Key만을 받게 설계
		**************************************************************/
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;
			// 가져온 Texture의 Key 문자열을 세팅
			SetTextureName(pAsset->GetKey());
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Drag and drop an image from the ContentUI\nTexture list into the box above.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Sprite name to save
	OutputTitle("Sprite name to save", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string spriteName = string(m_SpriteName.begin(), m_SpriteName.end());
	if (ImGui::InputTextWithHint("##SPRITENAMETOSAVE", "Name your Sprite", &spriteName))
	{
		wstring wspriteName = wstring(spriteName.begin(), spriteName.end());
		SetSpriteName(wspriteName);
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region SliceUV
	OutputTitle("SliceUV", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	if (ImGui::DragFloat2("##SLICEUV", m_SliceUV, 1.f, 0.f, FLT_MAX))
	{
		SetSliceUV(m_SliceUV);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region StartLoop
	OutputTitle("StartLoop", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##STARTLOOP", &m_StartLoop, 1.f, 0, INT_MAX))
	{
		SetStartLoop(m_StartLoop);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region EndLoop
	OutputTitle("EndLoop", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##ENDLOOP", &m_EndLoop, 1.f, 0, INT_MAX))
	{
		SetEndLoop(m_EndLoop);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Origin Index
	OutputTitle("Origin Index", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##ORIGINIDX", &m_OriIdx, 1.f, 0, INT_MAX))
	{
		SetOriIdx(m_OriIdx);
	}
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Start Y (Texture Slicing)");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion
	
	#pragma region End Origin Index
	OutputTitle("End Origin Index", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##ENDORIGINIDX", &m_EndOriIdx, 1.f, 0, INT_MAX))
	{
		SetEndOriIdx(m_EndOriIdx);
	}
	IMGUI_OPTIONAL()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
			"When setting the End Origin Index value,\nLoop from Origin Index to End Origin Index to produce the sprites.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Row
	OutputTitle("Row", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##ROW", &m_Row, 1.f, 0, INT_MAX))
	{
		SetRow(m_Row);
	}
	IMGUI_OPTIONAL()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Only for TileMap Sprites.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Col
	OutputTitle("Col", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##COL", &m_Col, 1.f, 0, INT_MAX))
	{
		SetCol(m_Col);
	}
	IMGUI_OPTIONAL()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Only for TileMap Sprites.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

}