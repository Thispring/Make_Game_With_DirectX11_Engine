#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"
#include "GameObject.h"

#include "imguiFunc.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	CreateChildUI();
	SetTargetObject(nullptr);
}

Inspector::~Inspector()
{
}


void Inspector::Tick_UI()
{
	// m_TargetObject로 받아온 GameObject 정보를 ImGui에 출력
	// GameObject의 정보 출력은 Inspector에서 직접처리하지 않고,
	// Inspector의 자식 UI클래스에서 담당합니다.
	if (m_TargetObject == nullptr)
		return;

	wstring Name = m_TargetObject->GetName();
	string strName = string(Name.begin(), Name.end());

	if (strName.empty())
		strName = "No Name";

	ImGui::Button(strName.c_str());

	// 위 버튼과 같은 라인 끝쪽에 삭제 버튼 추가하기
	ImGui::SameLine(250.f);

	if (ImGuiFunc::ColoredButton("Destroy", ImVec4(1.f, 0.2f, 0.2f, 1.f), ImVec2(100.f, 20.f)))
	{
		ImGui::OpenPopup("Destroy_GameObject?");
	}

	//// PushStyleColor로 Button 색상 바꾸기
	//ImVec4 col = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // 원하는 빨간색
	//ImVec4 col_hover = ImVec4(0.9f, 0.15f, 0.15f, 1.0f);
	//ImVec4 col_active = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
	//ImVec4 text_col = ImVec4(1, 1, 1, 1); // 텍스트를 흰색으로

	//ImGui::PushStyleColor(ImGuiCol_Button, col);
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hover);
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);
	//ImGui::PushStyleColor(ImGuiCol_Text, text_col);

	//if (ImGui::Button("Destroy", ImVec2(100.0f, 20.0f)))
	//{
	//	// 클릭 처리
	//	// 버튼을 누르면 팝업 상태를 'Open'으로 설정
	//	ImGui::OpenPopup("Destroy_GameObject?");
	//}

	//ImGui::PopStyleColor(4);


	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Destroy_GameObject?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want destroy this GameObject?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// GameObject Destroy 함수 호출
			m_TargetObject->Destroy();
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
}

/****************************************************************************
* Inspector에서 Target되고 있는 UI는 하나만 존재해야 하므로,
* Set함수에서 Object의 Component, Asset 둘 중 하나만 표시하도록 조건을 설정합니다.
****************************************************************************/
void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		// 자식 UI 클래스의 멤버에도 오브젝트 정보를 등록
		m_arrComUI[i]->SetTarget(m_TargetObject);
	}		

	// TargetObject가 Script를 얼마나 가지고 있는지 검사, 등록
	if (m_TargetObject != nullptr)
	{
		// 오브젝트의 Script에 대응하는 ScriptUI를 활성/비활성화
		const vector<Ptr<CScript>>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트가 보유한 Script 개수에 비해서 대응할 ScriptUI의 개수가 모자르면
		// 동적으로 추가합니다.
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			int AddCount = vecScripts.size() - m_vecScriptUI.size();

			for (int i = 0; i < AddCount; ++i)
			{
				EScriptUI* pScriptUI = new EScriptUI;
				pScriptUI->SetSizeAsChild(Vec2(0.f, 150.f));
				AddChildUI(pScriptUI);

				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 오브젝트에서 가져온 Script를 각각의 ScriptUI에 세팅
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (vecScripts.size() <= i)
				m_vecScriptUI[i]->SetScript(nullptr);
			else
				m_vecScriptUI[i]->SetScript(vecScripts[i].Get());
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI를 비활성화
	m_TargetAsset = nullptr;
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
		m_arrAssetUI[i]->SetActive(false);

}

void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
	// ComponentUI 비활성화
	SetTargetObject(nullptr);

	// 입력으로 들어온 Asset UI를 활성화
	m_TargetAsset = _Asset;
	
	// 들어온 정보가 null이 아닐때만 활성화
	if (m_TargetAsset == nullptr)
	{
		for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
			m_arrAssetUI[i]->SetActive(false);
	}
	else
	{
		ASSET_TYPE Type = m_TargetAsset->GetType();
		m_arrAssetUI[(UINT)Type]->SetActive(true);
		m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
	}
}
