#include "pch.h"
#include "ELevelUI.h"

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
}
