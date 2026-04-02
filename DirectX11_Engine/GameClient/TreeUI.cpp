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
	// 1. 테이블의 다음 행(Row) 시작
	ImGui::TableNextRow();

	// 2. 첫 번째 컬럼(Name + Tree 구조) 시작
	ImGui::TableNextColumn();

	// ---------------------------------------------------------
	// Flag 설정
	// ---------------------------------------------------------
	// Table 안에서는 SpanFullWidth 대신 SpanAllColumns를 주로 씁니다.
	UINT Flags = ImGuiTreeNodeFlags_SpanAllColumns      // 전체 컬럼에 걸쳐 하이라이트
		| ImGuiTreeNodeFlags_OpenOnDoubleClick          // 더블 클릭으로 열기
		| ImGuiTreeNodeFlags_OpenOnArrow;               // 화살표로 열기

	if (vecChildNode.empty())
		Flags |= ImGuiTreeNodeFlags_Leaf;

	if (m_Owner->GetSelected() == this)
		Flags |= ImGuiTreeNodeFlags_Selected;

	if (Framed)
		Flags |= ImGuiTreeNodeFlags_Framed;

	// ---------------------------------------------------------
	// 노드 그리기 (1열)
	// ---------------------------------------------------------
	// NodeName 생성 (Key는 ID 구별용으로 숨겨서 붙입니다)
	//string NodeName = Str + Key;

	char buffer[255]; // 충분한 크기의 배열 준비
	snprintf(buffer, sizeof(buffer), "%s%s", Str.c_str(), Key.c_str());
	const char* NodeName = buffer;

	// 자식 여부에 따라 TreeNodeEx 호출
	// 중요: TreeNodeEx는 화면에 아이템을 그리고, 열림 여부(bool)를 반환합니다.
	//bool isOpen = ImGui::TreeNodeEx(NodeName.c_str(), Flags);
	bool isOpen = ImGui::TreeNodeEx(NodeName, Flags);

	// ---------------------------------------------------------
	// 상호작용 (Click, Drag, Drop) - TreeNodeEx 직후에 한 번만 호출!
	// ---------------------------------------------------------
	// 기존 코드의 if/else 중복을 제거하고 여기서 통합 처리합니다.
	// ImGui는 가장 마지막에 그려진 아이템(TreeNode)을 대상으로 동작합니다.
	ClickCheck();
	DragCheck();
	DropCheck();

	// ---------------------------------------------------------
	// 추가 정보 그리기 (2열 - Key값 혹은 데이터 정보 보여주기)
	// ---------------------------------------------------------
	char KeyBuf[255]; // 충분한 크기의 배열 준비
	snprintf(KeyBuf, sizeof(KeyBuf), "%s%s", Str.c_str(), Key.c_str());
	const char* KeyName = KeyBuf;

	ImGui::TableNextColumn();
	//ImGui::TextDisabled("%s", Key.c_str()); // 예시: 2번째 칸에 Key값(ID)을 흐리게 출력
	ImGui::TextDisabled("%s", KeyName); // 예시: 2번째 칸에 Key값(ID)을 흐리게 출력

	// ---------------------------------------------------------
	// 자식 노드 순회
	// ---------------------------------------------------------
	if (isOpen)
	{
		// 자식이 있다면 재귀적으로 Tick 호출 (TableNextRow가 내부에서 호출됨)
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->Tick();
		}
		ImGui::TreePop();
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
	Ptr<TreeNode> pNewNode = NEW TreeNode;
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
	// 테이블 플래그 설정
	// Resizable: 컬럼 크기 조절 가능
	// Borders: 테두리 표시
	// RowBg: 행마다 배경색 교차 (가독성 향상)
	static ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;

	// 테이블 시작 (컬럼 2개: Name, Key)
	if (ImGui::BeginTable("TreeTable", 2, table_flags))
	{
		// 헤더 설정
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide); // 숨김 불가
		ImGui::TableSetupColumn("ID (Key)", ImGuiTableColumnFlags_WidthFixed, 100.0f); // 고정 너비
		ImGui::TableHeadersRow(); // 헤더 그리기

		// 루트 노드들 순회
		for (size_t i = 0; i < m_vecNode.size(); ++i)
		{
			m_vecNode[i]->Tick();
		}

		ImGui::EndTable();
	}

	// 기존 드래그 앤 드롭 마무리 로직
	if (m_DragNode.Get() && m_DropNode.Get()
		|| (m_DragNode.Get() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		if (m_DDInst && m_DDMemFunc)
		{
			(m_DDInst->*m_DDMemFunc)((DWORD_PTR)m_DragNode.Get(), (DWORD_PTR)m_DropNode.Get());
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}

}

#pragma endregion