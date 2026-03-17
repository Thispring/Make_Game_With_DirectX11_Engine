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

	// 컴포넌트 제거 버튼
	DeleteComponent(COMPONENT_TYPE::FLIPBOOK_RENDER);

	/***********************************************************************
	* Flipbook Render Component 정보를 가져옵니다.
	* EComponentUI 클래스에 GameObject Ptr을 반환하는 GetTarget 함수 활용
	* EComponentUI 클래스는 멤버로 GameObject를 Ptr로 가리키는 m_Target을 보유
	***********************************************************************/
	Ptr<CFlipbookRender> pFlipbookRender = GetTarget()->FlipbookRender();
	
	if (pFlipbookRender == nullptr)
		return;

	/************************************************************************
	* 재생 중인 Flipbook 멤버 정보 표시
	* CurFlipbook을 지정하면, 해당 Flipbook의 FPS, 반복횟수, Sprite 개수 표시
	* CurFlipbook은 m_vecFlipbook size로 지정
	************************************************************************/
	float fps = pFlipbookRender->GetFPS();
	int repeatCount = pFlipbookRender->GetRepeatCount();
	int curIndex = pFlipbookRender->GetCurFlipbook();
	// Flipbook 전체 목록을 vector로 가져와 ListUI에 전달
	m_vecFlipbook = pFlipbookRender->GetVecFlipbook();

	// Play, Stop Btn
	if (ImGui::Button("Play", ImVec2(120, 0)))
	{
		pFlipbookRender->SetIsStop(false);
	}
	ImGui::SameLine(150);
	if (ImGui::Button("Stop", ImVec2(120, 0)))
	{
		pFlipbookRender->SetIsStop(true);
	}
	SPACING_UI(5);


	// FPS
	ImGui::Text("FPS");
	ImGui::SameLine(150);
	if (ImGui::DragFloat("##FPS", &fps, 1.f, 0, FLT_MAX))
	{
		pFlipbookRender->SetFPS(fps);
		pFlipbookRender->Play(curIndex, fps, repeatCount);
	}
	SPACING_UI(5);


	// 반복횟수
	ImGui::Text("Repeat Count");
	ImGui::SameLine(150);
	if (ImGui::DragInt("##REPEATCOUNT", &repeatCount, 1.f, -1, INT_MAX))
	{
		pFlipbookRender->SetRepeatCount(repeatCount);
		pFlipbookRender->Play(curIndex, fps, repeatCount);
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Set to -1 for infinite loop.");
	SPACING_UI(5);


	// 현재 재생 중인 Flipbook 인덱스
	for (int i = 0; i < m_vecFlipbook.size(); ++i)
	{
		string curflipbookName = string(m_vecFlipbook[i]->GetKey().begin(), m_vecFlipbook[i]->GetKey().end());
		if (ImGui::RadioButton(curflipbookName.c_str(), &curIndex, i))
		{
			// 버튼을 눌렀다면, i번째를 현재 인덱스로 설정
			pFlipbookRender->SetCurFlipbook(i);

			/**********************************************************
			* 바로 Play를 해줘야, 주소 참조 문제를 방지할 수 있음
			* 이전에 재생중인 Flipbook의 Sprite 재생 인덱스와 
			* 이 시점에서 바뀐 Flipbook의 Sprite 재생 인덱스가 다를 수 있음
			**********************************************************/
			pFlipbookRender->Play(curIndex, fps, repeatCount);
		}
	}
	SPACING_UI(5);


	// NOTE(26-03-13):
	// RadioButton으로 Play 전환을 구현, 아래 실제 재생중인지
	// 확인용으로 남겨두고, 추후 제거
	// 
	// Flipbook Asset 목록을 가져옵니다.
	//ImGui::Text("Flipbook List");

	//// Flipbook은 어떤 Flipbook을 가리키는지 여부가 중요
	//// Flipbook 전체 목록을 vector로 가져와 ListUI에 전달
	//m_vecFlipbook = pFlipbookRender->GetVecFlipbook();

	//string FlipbookKey = "";
	//for (UINT i = 0; i < m_vecFlipbook.size(); i++)
	//{
	//	// 만약 재생중인 Flipbook이 있다면 HighLight로 구분
	//	if (i == pFlipbookRender->GetCurFlipbook())
	//	{
	//		// 여기에 들어왔다는 건 현재 재생중인 인덱스와 찾는 인덱스가 일치
	//		FlipbookKey = string(m_vecFlipbook[i]->GetKey().begin(), m_vecFlipbook[i]->GetKey().end());

	//		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(25, 145, 225, 255)); 
	//		OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
	//		ImGui::PopStyleColor();
	//	}
	//	else
	//	{
	//		FlipbookKey = string(m_vecFlipbook[i]->GetKey().begin(), m_vecFlipbook[i]->GetKey().end());
	//		OutputTitle(FlipbookKey, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
	//	}

	//}
	//SPACING_UI(5);


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
			{
				// vecFlipbook가 아예 없을때 추가를 허용하고,
				// 1개 이상일때 같은 이름이 있는지 검사 
				if (m_vecFlipbook.size() == 0)
				{
					// m_vecFlipbook은 멤버 변수
					// pFlipbookRender는 지역 변수
					// 서로 역할과 목적이 다름
					pFlipbookRender->AddFlipbook((AFlipbook*)pAsset.Get());
					return;
				}

				vector<Ptr<AFlipbook>>::iterator iter = m_vecFlipbook.begin();
				bool isDuplicate = false;

				// 기존에 추가되었던 Flipbook인지 Key 문자열로 비교해서
				// 이미 추가된 Flipbook이면 추가하지 않는다.
				for (; iter < m_vecFlipbook.end(); ++iter)
				{
					if (iter->Get()->GetKey() == pAsset->GetKey())
					{
						isDuplicate = true;
						break;
					}
				}

				if (!isDuplicate)
					pFlipbookRender->AddFlipbook((AFlipbook*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Drag and drop a Flipbook here to add.");
	SPACING_UI(5);
	ImGui::Separator();

}
