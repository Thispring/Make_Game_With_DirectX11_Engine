#include "pch.h"
#include "EMeshRenderUI.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "ListUI.h"

EMeshRenderUI::EMeshRenderUI()
	: EComponentUI(COMPONENT_TYPE::MESHRENDER, "MeshRenderUI")
{
}

EMeshRenderUI::~EMeshRenderUI()
{
}

void EMeshRenderUI::Tick_UI()
{
	OutputTitle("MeshRender");

	Ptr<CMeshRender> pMeshRender = GetTarget()->MeshRender();

	// ====
	// Mesh
	// ====
	ImGui::Text("Mesh List");
	ImGui::SameLine(120);

	Ptr<AMesh> pMesh = pMeshRender->GetMesh();
	string MeshKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	ImGui::InputText("##MeshName", MeshKey.data(), MeshKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		/**************************************************************
		* 마우스가 때졌을 때의 조건, 전달한 Key값과 동일한지 확인
		* 
		* 동작 의도에 맞게, 어느 ImGui에서 전달한 Key인지를 조건으로 구별하여
		* MeshRenderUI이면 ContentUI의 Mesh 목록에 있는 Key만을 받게 설계
		**************************************************************/
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::MESH == pAsset->GetType())
				pMeshRender->SetMesh((AMesh*)pAsset.Get());
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		vector<wstring> vecMeshNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pUI->AddString(vecMeshNames);
		pUI->AddDelegate(this, (DELEGATE_1)&EMeshRenderUI::SelectMesh);
		pUI->SetActive(true);
	}

	// ========
	// Material
	// ========
	ImGui::Text("Material");
	ImGui::SameLine(120);

	Ptr<AMaterial> pMtrl = pMeshRender->GetMaterial();
	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::InputText("##MtrlName", MtrlKey.data(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 올려져있는지 확인
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::MATERIAL == pAsset->GetType())
				pMeshRender->SetMaterial((AMaterial*)pAsset.Get());
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Material List");

		vector<wstring> vecMtrlName;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlName);
		pUI->AddString(vecMtrlName);
		pUI->AddDelegate(this, (DELEGATE_1)&EMeshRenderUI::SelectMtrl);
		pUI->SetActive(true);
	}

}

void EMeshRenderUI::SelectMesh(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMesh> pMesh = FIND(AMesh, key);

	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void EMeshRenderUI::SelectMtrl(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMaterial> pMtrl = FIND(AMaterial, key);

	GetTarget()->MeshRender()->SetMaterial(pMtrl);
}
