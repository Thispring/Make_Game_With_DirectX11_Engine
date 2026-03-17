#include "pch.h"
#include "Inspector.h"

//==========
// Component
//==========
#include "ETransformUI.h"
#include "ECameraUI.h"
#include "ECollider2DUI.h"
#include "ELight2DUI.h"
#include "EMeshRenderUI.h"
#include "ESpriteRenderUI.h"
#include "EFlipbookRenderUI.h"
#include "ETileRenderUI.h"
#include "EBillboardRenderUI.h"

//======
// Asset
//======
#include "EMeshUI.h"
#include "EMaterialUI.h"
#include "ETextureUI.h"
#include "ESoundUI.h"
#include "EGraphicShaderUI.h"
#include "EComputeShaderUI.h"
#include "ELevelUI.h"
#include "ESpriteUI.h"
#include "EFlipbookUI.h"
#include "ETileMapUI.h"
#include "EPrefabUI.h"


#define ADD_COMPONENT_UI(ComponentType, type, Size) m_arrComUI[(UINT)ComponentType] = new type;\
													m_arrComUI[(UINT)ComponentType]->SetSizeAsChild(Size);\
													AddChildUI(m_arrComUI[(UINT)ComponentType].Get());

#define ADD_ASSET_UI(AssetType, type) m_arrAssetUI[(UINT)AssetType] = new type;\
									  AddChildUI(m_arrAssetUI[(UINT)AssetType].Get());

void Inspector::CreateChildUI()
{
	ADD_COMPONENT_UI(COMPONENT_TYPE::TRANSFORM, ETransformUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::COLLIDER2D, ECollider2DUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::CAMERA, ECameraUI, Vec2(0.f, 300.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::LIGHT2D, ELight2DUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::MESHRENDER, EMeshRenderUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::SPRITE_RENDER, ESpriteRenderUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::FLIPBOOK_RENDER, EFlipbookRenderUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::TILE_RENDER, ETileRenderUI, Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::BILLBOARD_RENDER, EBillboardRenderUI, Vec2(0.f, 200.f));

	ADD_ASSET_UI(ASSET_TYPE::MESH, EMeshUI);
	ADD_ASSET_UI(ASSET_TYPE::MATERIAL, EMaterialUI);
	ADD_ASSET_UI(ASSET_TYPE::TEXTURE, ETextureUI);
	ADD_ASSET_UI(ASSET_TYPE::SOUND, ESoundUI);
	ADD_ASSET_UI(ASSET_TYPE::GRAPHICSHADER, EGraphicShaderUI);
	ADD_ASSET_UI(ASSET_TYPE::COMPUTESHADER, EComputeShaderUI);
	ADD_ASSET_UI(ASSET_TYPE::LEVEL, ELevelUI);
	ADD_ASSET_UI(ASSET_TYPE::SPRITE, ESpriteUI);
	ADD_ASSET_UI(ASSET_TYPE::FLIPBOOK, EFlipbookUI);
	ADD_ASSET_UI(ASSET_TYPE::TILEMAP, ETileMapUI);
	ADD_ASSET_UI(ASSET_TYPE::PREFAB, EPrefabUI);
}