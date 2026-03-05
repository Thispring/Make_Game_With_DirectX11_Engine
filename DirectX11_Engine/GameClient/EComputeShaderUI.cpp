#include "pch.h"
#include "EComputeShaderUI.h"

EComputeShaderUI::EComputeShaderUI()
	: EAssetUI(ASSET_TYPE::COMPUTESHADER)
{
}

EComputeShaderUI::~EComputeShaderUI()
{
}

void EComputeShaderUI::Tick_UI()
{
	OutputTitle();
}
