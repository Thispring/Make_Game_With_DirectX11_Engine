#include "pch.h"
#include "SpriteMaker.h"

#include "AssetMgr.h"
// ImGui::InputText의 기본 정의는 char* 버퍼, 이를 해결하기 위한 헤더
#include "imgui/imgui_stdlib.h"

SpriteMaker::SpriteMaker()
	: EditorUI("SpriteMaker")
	, m_TextureName {}
	, m_SliceUV {}
	, m_StartLoop(0)
	, m_EndLoop(0)
	, m_SpriteName {}
	, m_OriIdx(0)
	, m_Row(0)
	, m_Col(0)

{
}

SpriteMaker::~SpriteMaker()
{
}

void SpriteMaker::SettingClear()
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
}

void SpriteMaker::Tick_UI()
{
	OutputTitle("Making Sprite", ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Texture Name
		ImGui::Text("Texture Name");
		// wstring -> string 변환
		string Name = string(m_TextureName.begin(), m_TextureName.end());
		// Name
		ImGui::SameLine(150);
		if (ImGui::InputText("##TEXTURENAME", &Name))
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
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), 
			"Drag and drop an image from the ContentUI\nTexture list into the box above.");
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// SliceUV
		ImGui::Text("SliceUV");
		ImGui::SameLine(150);
		if (ImGui::DragFloat2("##SLICEUV", m_SliceUV))
		{
			SetSliceUV(m_SliceUV);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// StartLoop
		ImGui::Text("StartLoop");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##STARTLOOP", &m_StartLoop))
		{
			SetStartLoop(m_StartLoop);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// EndLoop
		ImGui::Text("EndLoop");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##ENDLOOP", &m_EndLoop))
		{
			SetEndLoop(m_EndLoop);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// Sprite name to save
		ImGui::Text("Sprite name to save");
		// wstring -> string 변환
		string spriteName = string(m_SpriteName.begin(), m_SpriteName.end());
		ImGui::SameLine(150);
		if (ImGui::InputText("##SPRITENAMETOSAVE", &spriteName))
		{
			wstring wspriteName = wstring(spriteName.begin(), spriteName.end());
			SetSpriteName(wspriteName);
		}
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
			"Please enter the name you want to save.");
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// Origin Index
		ImGui::Text("Origin Index");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##ORIGINIDX", &m_OriIdx))
		{
			SetOriIdx(m_OriIdx);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// Row
		ImGui::Text("Row");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##ROW", &m_Row))
		{
			SetRow(m_Row);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// Col
		ImGui::Text("Col");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##COL", &m_Col))
		{
			SetCol(m_Col);
		}
		for (int i = 0; i < 5; ++i) ImGui::Spacing();


		// 아래 함수는 ImGui 버튼이 눌렸을 때, 델리게이트로 전달합니다.
		// 
		// 오버로딩된 함수 호출
		// void AssetMgr::CreateEngineSprite(wstring _Name, Vec2 _Slice, int _StartLoop, int _EndLoop, wstring _Path, float _Origin)
		// Path의 경우 다음과 같이 전달합니다: L"Sprite\\name_Action_"
		if (ImGui::Button("SaveSprite##SpriteSaveBtn"))
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
				AssetMgr::GetInst()->CreateEngineSprite(m_TextureName, m_SliceUV, m_StartLoop, m_EndLoop, m_SpriteName, m_OriIdx, m_Row, m_Col);
				SettingClear();
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

		for (int i = 0; i < 7; ++i) ImGui::Spacing();
	}

}