#include "pch.h"
#include "EFlipbookRenderUI.h"

#include "ListUI.h"
#include "EditorMgr.h"
#include "AssetMgr.h"

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


	// Flipbook Asset 목록을 가져옵니다.
	ImGui::Text("Flipbook List");

	// Flipbook은 어떤 Flipbook을 가리키는지 여부가 중요
	// Flipbook 전체 목록을 vector로 가져와 ListUI에 전달
	vector<Ptr<AFlipbook>> vecFlipbook = pFlipbookRender->GetVecFlipbook();

	string FlipbookKey = "";
	for (UINT i = 0; i < vecFlipbook.size(); i++)
	{
		// 만약 재생중인 Flipbook이 있다면 HighLight로 구분
		if (i == pFlipbookRender->GetCurIndex())
		{
			// 여기에 들어왔다는 건 현재 재생중인 인덱스와 찾는 인덱스가 일치
			FlipbookKey = string(vecFlipbook[i]->GetKey().begin(), vecFlipbook[i]->GetKey().end());

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(25, 145, 225, 255)); 
			OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
			ImGui::PopStyleColor();
		}
		else
		{
			FlipbookKey = string(vecFlipbook[i]->GetKey().begin(), vecFlipbook[i]->GetKey().end());
			OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
		}

	}

}
