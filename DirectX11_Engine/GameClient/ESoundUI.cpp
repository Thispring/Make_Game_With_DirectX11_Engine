#include "pch.h"
#include "ESoundUI.h"

ESoundUI::ESoundUI()
	: EAssetUI(ASSET_TYPE::SOUND)
{
}

ESoundUI::~ESoundUI()
{
}

void ESoundUI::Tick_UI()
{
	OutputTitle();
}
