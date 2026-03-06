#include "pch.h"
#include "ESpriteUI.h"
#include "ASprite.h"
#include "ATexture.h"

ESpriteUI::ESpriteUI()
	: EAssetUI(ASSET_TYPE::SPRITE)
{
}

ESpriteUI::~ESpriteUI()
{
}

void ESpriteUI::Tick_UI()
{
	OutputTitle();

	// Sprite가 가지고 있는 ATexutre 스마트포인터 멤버인 m_Atlas에 접근합니다.
	// Atlas는 여러 이미지를 모아둔 하나의 이미지 파일입니다. 따라서 Sprite 멤버 중
	// 해당 Atlas를 원점에서 얼마나 자를지에 대한 UV 정보를 가져와, ImGui에 Atlas를 UV만큼
	// 잘라내 표시합니다. Sprite를 만들 때, 자를 해상도를 멤버로 저장해놓고
	// Atlas의 해상도가 아닌, 몇 픽셀만큼 자를지를 위 멤버를 가져와 ImGui에 표시합니다.
	Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();

	string Key = string(pSprite->GetKey().begin(), pSprite->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##SpriteName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// Sprite 에 Atlas 텍스쳐가 세팅되어있는지 확인
	Ptr<ATexture> pAtlas = pSprite->GetAtlas();
	bool IsAtlas = pAtlas.Get();

	// Atlas 이름
	string AtlasName = "None";
	if (IsAtlas)
		AtlasName = string(pAtlas->GetKey().begin(), pAtlas->GetKey().end());

	ImGui::Text("Atlas Name");
	ImGui::SameLine(120);
	ImGui::InputText("##AtlasName", AtlasName.data(), AtlasName.length() + 1, ImGuiInputTextFlags_ReadOnly);


	// Sprite UV 정보
	Vec2 LeftTopUV = pSprite->GetLeftTopUV();
	Vec2 SliceUV = pSprite->GetSliceUV();
	Vec2 BackgroundUV = pSprite->GetBackgroundUV();
	Vec2 OffsetUV = pSprite->GetOffsetUV();

	ImGui::Text("LeftTop");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { LeftTopUV.x * (float)pAtlas->GetWidth()
						  ,  LeftTopUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##LeftTop", pixel))
			{
				pSprite->SetLeftTopUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##LeftTop", pixel);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Slice");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { SliceUV.x * (float)pAtlas->GetWidth()
						  ,  SliceUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Slice", pixel))
			{
				pSprite->SetSliceUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Slice", pixel);
		}
	}
	ImGui::EndDisabled();


	ImGui::Text("Background");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { BackgroundUV.x * (float)pAtlas->GetWidth()
						  ,  BackgroundUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Background", pixel))
			{
				pSprite->SetBackgroundUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}

		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Background", pixel);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Offset");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { OffsetUV.x * (float)pAtlas->GetWidth()
						  ,  OffsetUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Offset", pixel))
			{
				pSprite->SetOffsetUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Offset", pixel);
		}
	}
	ImGui::EndDisabled();

	// Sprite UV 정보
	//Vec2 LeftTopUV = pSprite->GetLeftTopUV();
	//Vec2 SliceUV = pSprite->GetSliceUV();
	//Vec2 BackgroundUV = pSprite->GetBackgroundUV();
	//Vec2 OffsetUV = pSprite->GetOffsetUV();


	// Sprite 이미지 표시
	// NOTE(26-03-06):
	// ImGui로의 UV 좌표 계산문제 있음
	// 
	// BackGroundUV, OffsetUV로 실제 Atlas보다 넓게 잡아서 렌더링 중이기 때문에
	// BackGroundUV 좌표를 고려하여 다시 계산해야함
	ImGui::ImageWithBg((ImTextureRef)pAtlas->GetSRV().Get()
		, ImVec2(200, 200)
		, LeftTopUV, SliceUV
		, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
}
