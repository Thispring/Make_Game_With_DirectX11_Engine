#include "pch.h"
#include "ETileMapUI.h"

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
}
