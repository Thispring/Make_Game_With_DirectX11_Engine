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


	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	if (ImGui::TreeNode("Tree view"))
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

		static ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;
		//ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &tree_node_flags_base, ImGuiTreeNodeFlags_SpanFullWidth);
		//ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanLabelWidth", &tree_node_flags_base, ImGuiTreeNodeFlags_SpanLabelWidth);
		//ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns", &tree_node_flags_base, ImGuiTreeNodeFlags_SpanAllColumns);
		//ImGui::CheckboxFlags("ImGuiTreeNodeFlags_LabelSpanAllColumns", &tree_node_flags_base, ImGuiTreeNodeFlags_LabelSpanAllColumns);
		//ImGui::SameLine();

		if (ImGui::BeginTable("3ways", 3, table_flags))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
			ImGui::TableHeadersRow();

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
					const bool is_folder = (node->ChildCount > 0);

					ImGuiTreeNodeFlags node_flags = tree_node_flags_base;
					if (node != &all_nodes[0])
						node_flags &= ~ImGuiTreeNodeFlags_LabelSpanAllColumns; // Only demonstrate this on the root node.

					if (is_folder)
					{
						bool open = ImGui::TreeNodeEx(node->Name, node_flags);
						if ((node_flags & ImGuiTreeNodeFlags_LabelSpanAllColumns) == 0)
						{
							ImGui::TableNextColumn();
							ImGui::TextDisabled("--");
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(node->Type);
						}
						if (open)
						{
							for (int child_n = 0; child_n < node->ChildCount; child_n++)
								DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
							ImGui::TreePop();
						}
					}
					else
					{
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
				}
			};
			static const MyTreeNode nodes[] =
			{
				{ "Root with Long Name",          "Folder",       -1,       1, 3    }, // 0
				{ "Music",                        "Folder",       -1,       4, 2    }, // 1
				{ "Textures",                     "Folder",       -1,       6, 3    }, // 2
				{ "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
				{ "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
				{ "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
				{ "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
				{ "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
				{ "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
			};

			MyTreeNode::DisplayNode(&nodes[0], nodes);

			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}