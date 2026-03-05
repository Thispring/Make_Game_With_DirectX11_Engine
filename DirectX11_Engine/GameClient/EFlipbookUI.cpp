#include "pch.h"
#include "EFlipbookUI.h"

EFlipbookUI::EFlipbookUI()
	: EAssetUI(ASSET_TYPE::FLIPBOOK)
{
}

EFlipbookUI::~EFlipbookUI()
{
}

void EFlipbookUI::Tick_UI()
{
	OutputTitle();
}
