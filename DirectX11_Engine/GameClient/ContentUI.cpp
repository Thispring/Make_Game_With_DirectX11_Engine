#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"
#include "Inspector.h"

#include "AssetMgr.h"
#include "EditorMgr.h"

ContentUI::ContentUI()
	: EditorUI("ContentUI")
{
	m_Tree = new TreeUI;
	m_Tree->SetSeparator(false);
	m_Tree->AddDyanmicSelect(this, (DELEGATE_1)&ContentUI::SelectAsset);
	AddChildUI(m_Tree.Get());

	// Asset의 내용을 Tree에 반영
	Renew();
}

ContentUI::~ContentUI()
{
}

void ContentUI::SelectAsset(DWORD_PTR _Asset)
{
	if (_Asset == 0)
		return;

	// 클릭한 노드가 들고 있는 Asset 주소값을 입력받는다.
	Ptr<Asset> pAsset = (Asset*)_Asset;

	// Inspector에 ContentUI에서 클릭된 Asset의 주소를 알려준다.
	Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
	assert(pInspector.Get());

	pInspector->SetTargetAsset(pAsset);
}

void ContentUI::Renew()
{
	// Tree 비우기
	m_Tree->Clear();

	// 에셋 종류별로 Tree에 추가
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
	{
		// 에셋의 이름에 해당하는 노드를 추가
		// (enum 타입을 문자열로 바꾸는 함수 이용)
		Ptr<TreeNode> pNode = m_Tree->AddItem(nullptr, ToString((ASSET_TYPE)i));
		pNode->SetFramed(true);

		// 해당 에셋에 있는 모든 이름을 받아와서 하위 자식으로 추가
		vector<wstring> vecNames;
		AssetMgr::GetInst()->GetAssetNames((ASSET_TYPE)i, vecNames);

		for (const auto& str : vecNames)
		{
			// Asset의 주소를 받아서 TreeUI에 전달
			Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset((ASSET_TYPE)i, str);
			m_Tree->AddItem(pNode, string(str.begin(), str.end()), (DWORD_PTR)pAsset.Get());
		}
	}
}

void ContentUI::Tick_UI()
{
	// 변경사항이 있다면 Renew 호출하여, 새롭게 추가된 UI를 갱신
	if (AssetMgr::GetInst()->IsChanged())
		Renew();
}