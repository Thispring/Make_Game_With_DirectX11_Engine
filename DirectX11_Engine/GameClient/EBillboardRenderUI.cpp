#include "pch.h"
#include "EBillboardRenderUI.h"

#include "EditorMgr.h"

EBillboardRenderUI::EBillboardRenderUI()
	: EComponentUI(COMPONENT_TYPE::BILLBOARD_RENDER, "BillboardRenderUI")
{
}

EBillboardRenderUI::~EBillboardRenderUI()
{
}

void EBillboardRenderUI::Tick_UI()
{
	OutputTitle("BillboardRender");

	Ptr<CBillboardRender> pBillboard = GetTarget()->BillboardRender();
	string textureName = {};

	if (pBillboard->GetTex() != nullptr)
		textureName = string(pBillboard->GetTex()->GetKey().begin(), pBillboard->GetTex()->GetKey().end());

	ImGui::Text("Curent Billboard Texture");
	if (ImGui::InputText("##BILLBOARD_TEXTURE_NAME", &textureName))
	{

	}
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				pBillboard->SetTex((ATexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
	SPACING_UI(5);

	
	// Billboard Scale
	Vec2 vScale = pBillboard->GetBillboardScale();
	ImGui::Text("Billboard Scale");
	ImGui::SameLine(150);
	if (ImGui::DragFloat2("##Billboard Scale", vScale, 1.f, 0, FLT_MAX))
	{
		pBillboard->SetBillboardScale(vScale);
	}
	SPACING_UI(5);
}