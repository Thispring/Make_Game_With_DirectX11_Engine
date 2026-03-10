#include "pch.h"
#include "SpriteMaker.h"

#include "AssetMgr.h"
#include "imgui/imgui_stdlib.h"

SpriteMaker::SpriteMaker()
	: EditorUI("SpriteMaker")
	, m_Name {}
	, m_SliceUV {}
	, m_StartLoop(0)
	, m_EndLoop(0)
	, m_Path {}
	, m_OriIdx(0)

	, m_SpriteName {}
	, m_FlipbookName {}
	, m_FlipLoop(0)
{
}

SpriteMaker::~SpriteMaker()
{
}

void SpriteMaker::SpriteSettingClear()
{
	// 모든 멤버의 값을 0으로 초기화 하는 함수
	m_Name = {};
	m_SliceUV = Vec2{ 0.f, 0.f };
	m_StartLoop = 0;
	m_EndLoop = 0;
	m_Path = {};
	m_OriIdx = 0;
}

void SpriteMaker::FlipbookSettingClear()
{
	m_SpriteName = {};
	m_FlipbookName = {};
	m_FlipLoop = 0;
}

void SpriteMaker::Tick_UI()
{
	OutputTitle("Making Sprite", ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Texture Name
		ImGui::Text("Texture Name");
		// wstring -> string 변환
		string Name = string(m_Name.begin(), m_Name.end());
		// Name
		ImGui::SameLine(150);
		if (ImGui::InputText("##TEXTURENAME", &Name))
		{
			wstring wName = wstring(Name.begin(), Name.end());
			SetName(wName);
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
				SetName(pAsset->GetKey());
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Spacing();
		ImGui::Text("File Name Example: \"Setting Texture Name\"");
		ImGui::Text("FIND(ATexture, \"Setting Texture Name\"");
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// SliceUV
		ImGui::Text("SliceUV");
		ImGui::SameLine(150);
		if (ImGui::DragFloat2("##SLICEUV", m_SliceUV))
		{
			SetSliceUV(m_SliceUV);
		}
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// StartLoop
		ImGui::Text("StartLoop");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##STARTLOOP", &m_StartLoop))
		{
			SetStartLoop(m_StartLoop);
		}
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// EndLoop
		ImGui::Text("EndLoop");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##ENDLOOP", &m_EndLoop))
		{
			SetEndLoop(m_EndLoop);
		}
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// File Path
		ImGui::Text("File Path");
		// wstring -> string 변환
		string Path = string(m_Path.begin(), m_Path.end());
		ImGui::SameLine(150);
		if (ImGui::InputText("##PATH", &Path))
		{
			wstring wPath = wstring(Path.begin(), Path.end());
			SetPath(wPath);
		}
		ImGui::Spacing();
		ImGui::Text("File Path Example: Sprite\\spriteName_");
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// Origin Index
		ImGui::Text("Origin Index");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##ORIGINIDX", &m_OriIdx))
		{
			SetOriIdx(m_OriIdx);
		}
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


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
				AssetMgr::GetInst()->CreateEngineSprite(m_Name, m_SliceUV, m_StartLoop, m_EndLoop, m_Path, m_OriIdx);
				SpriteSettingClear();
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

		for (int i = 0; i < 7; ++i)
			ImGui::Spacing();
	}

	OutputTitle("Making Flipbook", ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	if (ImGui::CollapsingHeader("Flipbook", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Sprite Name
		ImGui::Text("Sprite Name");
		// wstring -> string 변환
		string spriteName = string(m_SpriteName.begin(), m_SpriteName.end());
		// Name
		ImGui::SameLine(150);
		if (ImGui::InputText("##SPRITENAME", &spriteName))
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

				// 가져온 Texture의 Key 문자열을 세팅
				SetName(pAsset->GetKey());
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Spacing();
		ImGui::Text("Sprite Name Example: \"Sprite\\spriteName_\"");
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// FlipbookName
		ImGui::Text("FlipbookName");
		// wstring -> string 변환
		string fbName = string(m_FlipbookName.begin(), m_FlipbookName.end());
		ImGui::SameLine(150);
		if (ImGui::InputText("##FLIPBOOKNAME", &fbName))
		{
			wstring wfbName = wstring(fbName.begin(), fbName.end());
			SetFlipbookName(wfbName);
		}
		ImGui::Spacing();
		ImGui::Text("File Path Example: Flipbook\\flipbookName.flip");
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();


		// FlipLoop
		ImGui::Text("FlipLoop");
		ImGui::SameLine(150);
		if (ImGui::DragInt("##FLIPLOOP", &m_FlipLoop))
		{
			SetFlipLoop(m_FlipLoop);
		}
		for (int i = 0; i < 5; ++i)
			ImGui::Spacing();



		if (ImGui::Button("SaveFlipbook##FlipbookSaveBtn"))
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
				AssetMgr::GetInst()->CreateEngineFlipbook(m_SpriteName, m_FlipbookName, m_FlipLoop);
				FlipbookSettingClear();
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


		for (int i = 0; i < 7; ++i)
			ImGui::Spacing();
	}

	OutputTitle("Making TileMap", ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	if (ImGui::CollapsingHeader("TileMap", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int i = 0; i < 7; ++i)
			ImGui::Spacing();
	}
}