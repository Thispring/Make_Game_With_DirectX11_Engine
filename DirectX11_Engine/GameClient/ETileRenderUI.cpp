#include "pch.h"
#include "ETileRenderUI.h"

ETileRenderUI::ETileRenderUI()
	: EComponentUI(COMPONENT_TYPE::TILE_RENDER, "TileRenderUI")
{
}

ETileRenderUI::~ETileRenderUI()
{
}

void ETileRenderUI::Tick_UI()
{
	OutputTitle("TileRender");
}

