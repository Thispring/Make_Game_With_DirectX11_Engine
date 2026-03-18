#include "pch.h"
#include "EPrefabUI.h"

EPrefabUI::EPrefabUI()
	: EAssetUI(ASSET_TYPE::PREFAB)
{
}

EPrefabUI::~EPrefabUI()
{
}

void EPrefabUI::Tick_UI()
{
	OutputTitle();

}