#include "pch.h"
#include "EGraphicShaderUI.h"

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
}
