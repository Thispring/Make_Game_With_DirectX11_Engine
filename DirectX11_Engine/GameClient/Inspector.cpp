#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"
#include "GameObject.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	CreateChildUI();
	SetTargetObject(nullptr);
	//SetTargetObject(LevelMgr::GetInst()->FindObjectByName(L"Player"));
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
