#include "pch.h"
#include "GameObjectMaker.h"

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

#define ADD_COMPONENT_UI(ComponentType, type, Size) m_arrComUI[(UINT)ComponentType] = NEW type;\
													m_arrComUI[(UINT)ComponentType]->SetSizeAsChild(Size);\
													AddChildUI(m_arrComUI[(UINT)ComponentType].Get());

void GameObjectMaker::CreateChildUI()
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
}
