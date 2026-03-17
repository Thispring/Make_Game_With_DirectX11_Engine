#include "pch.h"
#include "ESpriteRenderUI.h"

#include "EditorMgr.h"

ESpriteRenderUI::ESpriteRenderUI()
	: EComponentUI(COMPONENT_TYPE::SPRITE_RENDER, "SpriteRenderUI")
{
}

ESpriteRenderUI::~ESpriteRenderUI()
{
}

void ESpriteRenderUI::Tick_UI()
{
	OutputTitle("SpriteRender");

	// 컴포넌트 제거 버튼
	DeleteComponent(COMPONENT_TYPE::SPRITE_RENDER);

	// SpriteRender를 가져와, InputText에 이름을 띄우고
	// Sprite를 변경할 수 있도록 합니다.
	Ptr<CSpriteRender> pSprite = GetTarget()->SpriteRender();
	string spriteName = {};

	// 세팅된 Sprite가 있을 때만
	if (pSprite->GetSprite() != nullptr)
		spriteName = string(pSprite->GetSprite()->GetKey().begin(), pSprite->GetSprite()->GetKey().end());
		
	ImGui::Text("Curent Sprite");
	if (ImGui::InputText("##CSR_SPRITENAME", &spriteName))
	{

	}
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::SPRITE == pAsset->GetType())
			{
				pSprite->SetSprite((ASprite*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
	SPACING_UI(5);


	// Mtrl
	// Render의 종류별로 재질을 미리 설정하는 경우가 있다.
	// 그런경우에는 어떤 재질을 사용하는지 이름만 표시
	//ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Material Setting(ReadOnly)");
	ImGui::Text("Material Setting(ReadOnly)");
	Ptr<AMaterial> pMtrl = GetTarget()->SpriteRender()->GetMaterial();
	string mtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::InputText("##CSR_MTRLName", &mtrlName);
	SPACING_UI(5);

	// Sprite 설정 전이라면 표시 X
	// Sprite 설정이 되었다면 표시
	if (pSprite->GetSprite() != nullptr)
	{
		// Sprite가 들고 있는 Texutre, UV 정보로 어떤 Sprite인지 표시
		string atlasName = string(pSprite->GetSprite()->GetAtlas()->GetKey().begin(), pSprite->GetSprite()->GetAtlas()->GetKey().end());
		Vec2 ltUV = pSprite->GetSprite()->GetLeftTopUV();
		Vec2 sUV = pSprite->GetSprite()->GetSliceUV();
		Vec2 bgUV = pSprite->GetSprite()->GetBackgroundUV();
		Vec2 osUV = pSprite->GetSprite()->GetOffsetUV();	
	
		//ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Texutre Info(ReadOnly)");
		ImGui::Text("Texutre Info(ReadOnly)");

		ImGui::Text("Atlas Name");
		ImGui::InputText("##CSR_ATLASNAME", &atlasName, ImGuiInputTextFlags_ReadOnly);
		SPACING_UI(2);

		ImGui::Text("Left Top UV");
		ImGui::InputFloat2("##LEFTTOP", ltUV, "%.3f", ImGuiInputTextFlags_ReadOnly);
		SPACING_UI(2);

		ImGui::Text("Slice UV");
		ImGui::InputFloat2("##SLICE", sUV, "%.3f", ImGuiInputTextFlags_ReadOnly);
		SPACING_UI(2);

		ImGui::Text("Background UV");
		ImGui::InputFloat2("##BACKGROUND", bgUV, "%.3f", ImGuiInputTextFlags_ReadOnly);
		SPACING_UI(2);

		ImGui::Text("Offset UV");
		ImGui::InputFloat2("##OFFSET", osUV, "%.3f", ImGuiInputTextFlags_ReadOnly);
		SPACING_UI(2);

	}

}
