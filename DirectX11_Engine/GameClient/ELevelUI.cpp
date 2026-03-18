#include "pch.h"
#include "ELevelUI.h"

#include "AssetMgr.h"
#include "EditorMgr.h"

ELevelUI::ELevelUI()
	: EAssetUI(ASSET_TYPE::LEVEL)
{
}

ELevelUI::~ELevelUI()
{
}

void ELevelUI::Tick_UI()
{
	OutputTitle();

	Ptr<ALevel> pLevel = (ALevel*)GetTargetAsset().Get();
	Layer* pLayer = pLevel->GetLayer();
	
	ImGui::Text("Layer List");
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		// 설정한 이름 문자열과, Layer 번호 (i) 출력
		string layerName = to_string(i) + "         ";
		layerName += string(pLayer[i].GetName().begin(), pLayer[i].GetName().end());
		char buffer[255];
		snprintf(buffer, sizeof(buffer), "##LAYER_NAME%d", i);
		const char* Lable = buffer;
		if (ImGui::InputText(Lable, &layerName))
		{
			// 이름이 없는 Layer 이름 설정할 수 있게 구현
			//pLayer[0].SetName(L"");			
		}

	}
	ImGui::Spacing();
	
}
