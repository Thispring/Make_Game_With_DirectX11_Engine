#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"
#include "Inspector.h"

#include "AssetMgr.h"
#include "LevelMgr.h"
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

// 이름에서 마지막 숫자 토큰(및 필요하면 바로 앞 구분자)을 제거하여 그룹 키 반환 함수
static string GetGroupKey(const wstring& nameW)
{
	// 확장자 분리
	size_t dot = nameW.find_last_of(L'.');
	wstring base = (dot == wstring::npos) ? nameW : nameW.substr(0, dot);
	wstring ext = (dot == wstring::npos) ? L"" : nameW.substr(dot);

	// 끝에서부터 연속된 숫자 제거
	size_t i = base.size();
	while (i > 0 && iswdigit(base[i - 1])) --i;

	// 숫자 바로 앞에 '_' 또는 '-' 가 있으면 그것도 제거
	if (i > 0 && (base[i - 1] == L'_' || base[i - 1] == L'-')) --i;

	wstring keyW = base.substr(0, i) + ext;

	// wstring -> string 변환 (간단히 narrow; 유니코드 필요하면 적절히 변환)
	return string(keyW.begin(), keyW.end());
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

	// NOTE(26-03-20): 아래 방식으로 UI를 띄울 수는 있지만
	// 원본이 건드려져, 문제가 발생할 수 있음
	// 다른 설계방식 고려해보기
	// 클릭한게 Prefab이라면, 오브젝트 정보를 Inspector에 전달
	//if (pAsset->GetType() == ASSET_TYPE::PREFAB)
	//{
	//	Ptr<APrefab> pPrefab = (APrefab*)pAsset.Get();
	//	pInspector->SetTargetObject(pPrefab->GetGameObject());
	//}
	//else
	//{
	//	pInspector->SetTargetAsset(pAsset);
	//}
	
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

		// 그룹핑: 마지막 연속된 숫자(및 바로 앞 구분자 '_' 또는 '-')를 제거한 키로 묶음
		std::map<string, vector<wstring>> groups;
		for (const auto& nameW : vecNames)
		{
			string key = GetGroupKey(nameW);
			groups[key].push_back(nameW);
		}

		// 그룹을 순회하며 Tree에 추가
		for (auto& pair : groups)
		{
			const string& groupKey = pair.first;
			vector<wstring>& members = pair.second;

			if (members.size() > 1)
			{
				// 그룹 노드 생성 (예: "cat_Idle")
				Ptr<TreeNode> groupNode = m_Tree->AddItem(pNode, groupKey);
				groupNode->SetFramed(false);

				// 그룹 멤버들을 child로 추가
				for (const auto& memberW : members)
				{
					string displayName(memberW.begin(), memberW.end());
					Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset((ASSET_TYPE)i, memberW);
					m_Tree->AddItem(groupNode, displayName, (DWORD_PTR)(pAsset.Get()));
				}
			}
			else
			{
				// 단일 항목은 바로 추가
				const wstring& memberW = members[0];
				string displayName(memberW.begin(), memberW.end());
				Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset((ASSET_TYPE)i, memberW);
				m_Tree->AddItem(pNode, displayName, (DWORD_PTR)(pAsset.Get()));
			}
		}
	}
}

void ContentUI::Tick_UI()
{
	// 변경사항이 있다면 Renew 호출하여, 새롭게 추가된 UI를 갱신
	if (AssetMgr::GetInst()->IsChanged())
	{
		Renew();
	}


	static ImGuiTableFlags table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

	static ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;

	if (ImGui::BeginTable("3ways", 3))
	{
		// Simple storage to output a dummy file-system.
		struct MyTreeNode
		{
			const char* Name;
			const char* Type;
			int             Size;
			int             ChildIdx;
			int             ChildCount;
			static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGuiTreeNodeFlags node_flags = tree_node_flags_base;
				if (node != &all_nodes[0])
					node_flags &= ~ImGuiTreeNodeFlags_LabelSpanAllColumns; // Only demonstrate this on the root node.


				// [중요 1] UI ID 충돌 방지
				// 파일 이름이 같더라도 인덱스가 다르면 다른 위젯으로 인식하게 함
				int node_idx = (int)(node - all_nodes); // 인덱스 계산
				ImGui::PushID(node_idx);                // ID 스택에 인덱스 추가

				// 1. 트리 노드 그리기
				// (이제 이름이 같아도 PushID 덕분에 별개의 노드로 작동합니다)
				ImGui::TreeNodeEx(node->Name, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);

				// 2. Drag Source 부착
				if (ImGui::BeginDragDropSource())
				{
					// [중요 2] Payload 설정 (인덱스를 정수로 넘김)
					// 받는 쪽에서는 이 int 값을 꺼내서 all_nodes[idx]로 접근하면 됨
					ImGui::SetDragDropPayload("DND_NODE", &node_idx, sizeof(int));

					// 툴팁 표시
					ImGui::Text("Moving: %s (Idx: %d)", node->Name, node_idx);

					ImGui::EndDragDropSource();
				}

				// [중요 1-2] ID 스택 복구 (반드시 짝을 맞춰야 함)
				ImGui::PopID();

				// 3. 나머지 컬럼 그리기
				ImGui::TableNextColumn();
				ImGui::Text("%d", node->Size);
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(node->Type);
			}
		};

		ImGui::EndTable();

	}
}
