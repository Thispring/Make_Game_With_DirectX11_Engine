#include "pch.h"
#include "ETileMapUI.h"
#include "AssetMgr.h"

ETileMapUI::ETileMapUI()
	: EAssetUI(ASSET_TYPE::TILEMAP)
{
}

ETileMapUI::~ETileMapUI()
{
}

void ETileMapUI::Tick_UI()
{
	OutputTitle();

	Ptr<ATileMap> pTileMap = (ATileMap*)GetTargetAsset().Get();
	string Key = string(pTileMap->GetKey().begin(), pTileMap->GetKey().end());

	vector<Ptr<ASprite>> pSprite = pTileMap->GetSprites();
	Vec2 vSize = pTileMap->GetTileSize();
	UINT row = pTileMap->GetRow();
	UINT col = pTileMap->GetCol();

	ImGui::Text("Name");
	ImGui::InputText("##TileMapName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Row");
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	ImGui::InputInt("##Row", (int*)&row, 0, 0, ImGuiInputTextFlags_ReadOnly);
	
	ImGui::Text("Col");
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	ImGui::InputInt("##Col", (int*)&col, 0, 0, ImGuiInputTextFlags_ReadOnly);


	int Count = 0;
	for (UINT i = 0; i < col; ++i)
	{
		// 줄이 바뀔 때마다 ID 충돌 방지를 위한 Group ID
		ImGui::PushID(i);

		for (UINT j = 0; j < row; ++j)
		{
			// 첫 번째 열(j=0)이 아닐 때만 SameLine (옆으로 붙이기)
			if (j > 0) ImGui::SameLine();

			// 각 이미지마다 고유 ID 부여
			ImGui::PushID(j);

			// 인덱스 범위 체크 
			if (Count < pSprite.size())
			{
				Vec2 LeftTopUV = pSprite[Count]->GetLeftTopUV();
				Vec2 SliceUV = pSprite[Count]->GetSliceUV();

				// 이미지 그리기
				ImGui::Image((ImTextureRef)pSprite[Count]->GetAtlas()->GetSRV().Get(), vSize * 2, LeftTopUV, LeftTopUV + SliceUV);
			}

		ImGui::PopID(); // 안쪽 ID Pop

		// 이미지 하나 처리 후 Count 증가
		Count++;
		}

		ImGui::PopID(); // 바깥쪽 ID Pop (Row ID)
	}
}
