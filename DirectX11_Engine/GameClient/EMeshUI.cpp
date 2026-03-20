#include "pch.h"
#include "EMeshUI.h"
#include "AMesh.h"

EMeshUI::EMeshUI()
	: EAssetUI(ASSET_TYPE::MESH)
{
}

EMeshUI::~EMeshUI()
{
}

void EMeshUI::Tick_UI()
{
	OutputTitle();

	// Mesh UI에는 정점 개수와, 인덱스 개수를 표시
	// 생성되는 매쉬의 프리뷰를 표시
	Ptr<AMesh> pMesh = (AMesh*)GetTargetAsset().Get();
	string Name = string(pMesh->GetKey().begin(), pMesh->GetKey().end());


	//const Vtx* pVtx = pMesh->GetVtxSysMem();
	//const UINT* pUINT = pMesh->GetIdxSysMem();

	UINT Vtx = pMesh->GetVtxCount();
	UINT Idx = pMesh->GetIdxCount();

	ImGui::Text(Name.c_str());
	SPACING_UI(3);

	ImGui::Text("Vtx Count ");
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	ImGui::DragInt("##VTX", (int*)&Vtx, 1.0f, 0, 0, "%d", ImGuiInputTextFlags_ReadOnly);
	SPACING_UI(3);

	ImGui::Text("Index Count ");
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	ImGui::DragInt("##IDX", (int*)&Idx, 1.0f, 0, 0, "%d", ImGuiInputTextFlags_ReadOnly);
	SPACING_UI(3);
}
