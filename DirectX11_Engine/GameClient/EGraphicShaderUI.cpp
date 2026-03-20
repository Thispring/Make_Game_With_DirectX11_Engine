#include "pch.h"
#include "EGraphicShaderUI.h"
#include "AssetMgr.h"

EGraphicShaderUI::EGraphicShaderUI()
	: EAssetUI(ASSET_TYPE::GRAPHICSHADER)
{
}

EGraphicShaderUI::~EGraphicShaderUI()
{
}

void EGraphicShaderUI::Tick_UI()
{
	OutputTitle();

	Ptr<AGraphicShader> pShader = (AGraphicShader*)GetTargetAsset().Get();
	string Name = string(pShader->GetKey().begin(), pShader->GetKey().end());

	ImGui::Text(Name.c_str());
}
