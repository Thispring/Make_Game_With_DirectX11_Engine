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
	ImGui::SameLine();
	ImGui::Text("Vtx Count ");
	ImGui::SameLine();
	ImGui::DragInt("##VTX", (int*)&Vtx);
	ImGui::Spacing();
	ImGui::Text(Name.c_str());
	ImGui::SameLine();
	ImGui::Text("Index Count ");
	ImGui::SameLine();
	ImGui::DragInt("##IDX", (int*)&Idx);
}
