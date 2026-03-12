#include "pch.h"
#include "EFlipbookRenderUI.h"

#include "ListUI.h"
#include "EditorMgr.h"
#include "AssetMgr.h"

#include "imgui/imgui_stdlib.h"

EFlipbookRenderUI::EFlipbookRenderUI()
	: EComponentUI(COMPONENT_TYPE::FLIPBOOK_RENDER, "FlipbookRenderUI")
{
}

EFlipbookRenderUI::~EFlipbookRenderUI()
{
}

void EFlipbookRenderUI::Tick_UI()
{
	OutputTitle("FlipbookRender");

	// Flipbook Render Component 정보를 가져옵니다.
	// EComponentUI 클래스에 GameObject Ptr을 반환하는 GetTarget 함수 활용
	// EComponentUI 클래스는 멤버로 GameObject를 Ptr로 가리키는 m_Target을 보유
	Ptr<CFlipbookRender> pFlipbookRender = GetTarget()->FlipbookRender();
	

	// 재생 중인 Flipbook 멤버 정보 표시




	// Flipbook Asset 목록을 가져옵니다.
	ImGui::Text("Flipbook List");

	// Flipbook은 어떤 Flipbook을 가리키는지 여부가 중요
	// Flipbook 전체 목록을 vector로 가져와 ListUI에 전달
	m_vecFlipbook = pFlipbookRender->GetVecFlipbook();

	string FlipbookKey = "";
	for (UINT i = 0; i < m_vecFlipbook.size(); i++)
	{
		// 만약 재생중인 Flipbook이 있다면 HighLight로 구분
		if (i == pFlipbookRender->GetCurFlipbook())
		{
			// 여기에 들어왔다는 건 현재 재생중인 인덱스와 찾는 인덱스가 일치
			FlipbookKey = string(m_vecFlipbook[i]->GetKey().begin(), m_vecFlipbook[i]->GetKey().end());

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(25, 145, 225, 255)); 
			OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
			ImGui::PopStyleColor();
		}
		else
		{
			FlipbookKey = string(m_vecFlipbook[i]->GetKey().begin(), m_vecFlipbook[i]->GetKey().end());
			OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
		}

	}
	SPACING_UI(5);


	// Add Flipbook
	ImGui::Text("Add Flipbook");
	// wstring -> string 변환
	string flipbookName = string(m_AddFlipbookName.begin(), m_AddFlipbookName.end());
	if (ImGui::InputText("##ADDFLIPBOOK", &flipbookName))
	{
		wstring wflipbookName = wstring(flipbookName.begin(), flipbookName.end());
		SetAddFlipbookName(wflipbookName);
	}
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			// Flipbook이 들어왔다면, List에 새로 추가
			if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
				pFlipbookRender->AddFlipbook((AFlipbook*)pAsset.Get());
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"");
	SPACING_UI(5);
	ImGui::Separator();

}
