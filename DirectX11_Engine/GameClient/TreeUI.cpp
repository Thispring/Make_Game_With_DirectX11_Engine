#include "pch.h"
#include "TreeUI.h"

#pragma region Struct_TreeNode
//=========
// TreeNode
//=========
// TreeNode 구조체의 생성자에서
// Entity에 저장된 고유의 ID값을 가져옵니다.
TreeNode::TreeNode()
	: Parent(nullptr)
	, m_Owner(nullptr)
	, Framed(false)
{
	// 각 노드가 표기하려는 이름이 같을 수가 있기 때문에, 보여주려는 이름 뒤에 붙을 고유 문자열을 미리 생성해둔다.
	char szKey[50] = {};
	sprintf_s(szKey, 50, "##TreeNode%d", GetID());
	Key = szKey;
}

TreeNode::~TreeNode()
{
}

void TreeNode::ClickCheck()
{
	// 마우스가 올려져 있고, 이전에 클릭되었다면 변경
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_Owner->RegisterSelected(this);
	}
}

void TreeNode::DragCheck()
{
	// 드래그 앤 드롭 방식으로
	// 표시한 UI가 가리킨 데이터를 Inspector에 반환
	if (ImGui::BeginDragDropSource())
	{
		// 매개변수로 문자열 Key 값(받는 쪽에서 Key가 동일해야함)과
		// 전달한 데이터의 시작 주소(void 포인터)를 전달합니다.
		
		// Drag 사이에 Text를 넣으면, 드래그 중인 마우스 위치를 따라다니며 Text를 렌더링
		ImGui::Text(Str.c_str());

		if (Data != 0)
		ImGui::SetDragDropPayload(m_Owner->GetParentUI()->GetUIName().c_str()
									, &Data, sizeof(Data));

		m_Owner->RegisterDragged(this);

		ImGui::EndDragDropSource();
	}
}

void TreeNode::DropCheck()
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload(m_Owner->GetDropKey().c_str());

		if (Payload)
		{
			m_Owner->RegisterDropped(this);
		}

		ImGui::EndDragDropTarget();
	}
}

void TreeNode::Tick()
{
	// TreeNode Flag 설정 (ImGui쪽 enum 사용)
	UINT Flags = ImGuiTreeNodeFlags_SpanFullWidth		// 클릭 판정범위 확장
		| ImGuiTreeNodeFlags_OpenOnDoubleClick			// 더블 클릭으로만 열리기
		| ImGuiTreeNodeFlags_OpenOnArrow;				// 화살표 누르면 열리기


	// 노드가 자식노드를 보유하고 있지 않으면 Leaf 플래그 추가
	if (vecChildNode.empty())
		Flags |= ImGuiTreeNodeFlags_Leaf;

	// UI 자기 자신이 선택되었다면 Flag 변경
	if (m_Owner->GetSelected() == this)
		Flags |= ImGuiTreeNodeFlags_Selected;

	// Framed가 true인 UI만 적용
	if (Framed)
		Flags |= ImGuiTreeNodeFlags_Framed;

	string NodeName = Str + Key;

	// 자식이 없는 Node 중 Frame이 적용되었을 때,
	// 보여지는 텍스트의 시작지점을 맞춰주기 위한 코드
	if (Framed && vecChildNode.empty())
		NodeName = "   " + NodeName;

	// 트리노드에 등록한 문자열을 Key로 찾아서 출력
	if (ImGui::TreeNodeEx(NodeName.c_str(), Flags))
	{
		// 복사 붙여넣기 식 코드라면 함수화를 고려
		ClickCheck();
		// Drag & Drop의 판정은 Node에서 판별하고
		// 판정되었을 때의 기능은 상속을 받은 자식에서 구현합니다.
		DragCheck();
		DropCheck();

		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->Tick();
		}

		ImGui::TreePop();
	}
	else
	{
		ClickCheck();
		DragCheck();
		DropCheck();
	}
}

#pragma endregion


#pragma region Class_TreeUI
//=======
// TreeUI
//=======
TreeUI::TreeUI()
	: EditorUI("TreeUI")
	, m_Inst(nullptr)
	, m_MemFunc(nullptr)
	, m_DDInst(nullptr)
	, m_DDMemFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
}

Ptr<TreeNode> TreeUI::AddItem(Ptr<TreeNode> _ParentNode, string _Str, DWORD_PTR _Data)
{
	//=================================
	// Node 자료구조 방식의 등록과 유사하다
	//=================================
	Ptr<TreeNode> pNewNode = new TreeNode;
	pNewNode->Str = _Str;
	pNewNode->m_Owner = this;
	pNewNode->Data = _Data;


	// 최상위 부모노드로 추가
	if (_ParentNode == nullptr)
	{
		m_vecNode.push_back(pNewNode);
	}
	// 특정 노드 밑에 자식으로 추가
	else
	{
		_ParentNode->AddChildNode(pNewNode);
	}

	return pNewNode;
}

void TreeUI::RegisterSelected(Ptr<TreeNode> _Node)
{
	m_Selected = _Node;

	// DELEGATE_1에 등록된 정보만 Inspector에 표시
	if (m_Inst && m_MemFunc)
		(m_Inst->*m_MemFunc)(_Node->Data);
}

void TreeUI::Tick_UI()
{
	for (size_t i = 0; i < m_vecNode.size(); ++i)
	{
		m_vecNode[i]->Tick();
	}

	// Drag 하던 노드를 특정 노드에 Drop
	// OR 조건으로 Drag를 한 상태에서 마우스를 땐 상태였다면
	if (m_DragNode.Get() && m_DropNode.Get()
		|| (m_DragNode.Get() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		if (m_DDInst && m_DDMemFunc)
		{
			// Drag & Drop Node의 주소를 전달
			(m_DDInst->*m_DDMemFunc)((DWORD_PTR)m_DragNode.Get(), (DWORD_PTR)m_DropNode.Get());
		}

		// nullptr 초기화로 주소 비워주기
		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
}

#pragma endregion