#include "pch.h"
#include "ETileRenderUI.h"

#include "EditorMgr.h"

ETileRenderUI::ETileRenderUI()
	: EComponentUI(COMPONENT_TYPE::TILE_RENDER, "TileRenderUI")
{
}

ETileRenderUI::~ETileRenderUI()
{
}

void ETileRenderUI::Tick_UI()
{
	OutputTitle("TileRender");

	// UI에서 Drag Drop으로 ATileMap만 세팅 할 수 있도록합니다.
	Ptr<CTileRender> pTile = GetTarget()->TileRender();
	string tileName = {};

	// 처음엔 빈 상태로 띄우기,
	// InputText에 이름을 입력받거나, Drag Drop으로 이름을 가져와
	// SetTileMap 호출
	// 
	// void SetTileMap(Ptr<ATileMap> _TileMap);
	if (pTile->GetTileMap() != nullptr)
		tileName = string(pTile->GetTileMap()->GetKey().begin(), pTile->GetTileMap()->GetKey().end());

	ImGui::Text("Curent TileMap");
	if (ImGui::InputText("##TILEMAP_NAME", &tileName))
	{

	}
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::TILEMAP == pAsset->GetType())
			{
				pTile->SetTileMap((ATileMap*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
	SPACING_UI(5);

}

