#include "pch.h"
#include "EBillboardRenderUI.h"


EBillboardRenderUI::EBillboardRenderUI()
	: EComponentUI(COMPONENT_TYPE::BILLBOARD_RENDER, "BillboardRenderUI")
{
}

EBillboardRenderUI::~EBillboardRenderUI()
{
}

void EBillboardRenderUI::Tick_UI()
{
	OutputTitle("BillboardRender");
}