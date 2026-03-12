#include "pch.h"
#include "ESpriteRenderUI.h"

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
}
