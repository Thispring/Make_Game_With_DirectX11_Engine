#include "pch.h"
#include "Outliner.h"

#include "LevelMgr.h"
#include "TreeUI.h"

#include "EditorMgr.h"
#include "Inspector.h"


Outliner::Outliner()
	: EditorUI("Outliner")
{
	m_Tree = new TreeUI;
	m_Tree->AddDyanmicSelect(this, (DELEGATE_1)&Outliner::SelectGameObject);
	m_Tree->AddDyanmicDragDrop(this, (DELEGATE_2)&Outliner::AddChild);
	
	m_Tree->SetDropKey("Outliner");	// Self DragDrop 사용, 다른 창의 정보를 Drop하길 원하면, 해당 키값을 설정해주어야 함

	AddChildUI(m_Tree.Get());

}

Outliner::~Outliner()
{
}

void Outliner::AddGameObject(Ptr<TreeNode> _ParentNode, Ptr<GameObject> _Object)
{
	// 입력받은 GameObject의 이름을 받아와 TreeNode에 저장합니다.
	// 만약 자식이 있는 오브젝트라면, 재귀호출 방식을 사용하여 이 함수를 호출합니다.
	
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());

	if (ObjectName.empty())
		ObjectName = "No Name";

	Ptr<TreeNode> pNewNode = m_Tree->AddItem(_ParentNode, ObjectName.c_str(), (DWORD_PTR)_Object.Get());

	for (size_t i = 0; i < _Object->GetChild().size(); i++)
		AddGameObject(pNewNode, _Object->GetChild()[i]);

}

void Outliner::SelectGameObject(DWORD_PTR _Object)
{
	Ptr<GameObject> pSelectedObject = (GameObject*)_Object;

	Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
	assert(pInspector.Get());

	pInspector->SetTargetObject(pSelectedObject);
}

void Outliner::AddChild(DWORD_PTR _Src, DWORD_PTR _Dest)
{
	Ptr<TreeNode> pDragNode = (TreeNode*)_Src;
	Ptr<TreeNode> pDropNode = (TreeNode*)_Dest;

	Ptr<GameObject> ScrObj = (GameObject*)pDragNode->Data;
	Ptr<GameObject> DestObj = nullptr;

	if (pDropNode != nullptr)
		DestObj = (GameObject*)pDropNode->Data;

	// 목적지 Object가 null이면, Object가 없으므로 최상위 부모로 설정하고
	// null이 아니면 해당 Object의 자식으로 등록
	if (DestObj == nullptr)
	{
		// ScrObj가 자식타입의 오브젝트일 경우에만, 최상위 부모로 등록
		if (ScrObj->GetParent() != nullptr)
		{
			ScrObj->DisconnectWithParent();
			ScrObj->RegisterAsParent();
		}
	}
	else
	{
		DestObj->AddChild(ScrObj);
	}
}

void Outliner::Renew()
{
	// 트리에 표기된 오브젝트 정보를 전부 삭제
	m_Tree->Clear();

	// 현재 레벨을 가져온다
	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
	if (pLevel == nullptr)
		return;

	// 모든 레이어를 순회
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		// 각 레이어에 등록된 최상위 부모 오브젝트를 가져온다.
		const vector<Ptr<GameObject>>& vecParents = pLevel->GetLayer(i)->GetParentObjects();

		// 최상위 부모 오브젝트들을 트리에 추가합니다.
		for (const auto& Object : vecParents)
			AddGameObject(nullptr, Object);
	}
}

void Outliner::Tick_UI()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
	if (pCurLevel != nullptr)
	{
		if (pCurLevel->IsChanged())
		{
			Renew();
			Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
			pInspector->SetTargetObject(nullptr);

		}
	}
}
