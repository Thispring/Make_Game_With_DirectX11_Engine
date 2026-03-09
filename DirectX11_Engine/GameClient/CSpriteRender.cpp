#include "pch.h"
#include "CSpriteRender.h"
#include "AssetMgr.h"
#include "GameObject.h"

CSpriteRender::CSpriteRender()
	: CRenderComponent(COMPONENT_TYPE::SPRITE_RENDER)
{
}

CSpriteRender::~CSpriteRender()
{
}

void CSpriteRender::FinalTick()
{

}

void CSpriteRender::Render()
{
	// sprite.fx 에 특정 값을 전달
	GetMaterial()->SetTexture(TEX_0, m_Sprite->GetAtlas());
	GetMaterial()->SetScalar(VEC2_0, m_Sprite->GetLeftTopUV());
	GetMaterial()->SetScalar(VEC2_1, m_Sprite->GetSliceUV());
	GetMaterial()->SetScalar(VEC2_2, m_Sprite->GetBackgroundUV());
	GetMaterial()->SetScalar(VEC2_3, m_Sprite->GetOffsetUV());

	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CSpriteRender::CreateMaterial()
{

}

void CSpriteRender::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	SaveAssetRef(_File, m_Sprite.Get());
}

void CSpriteRender::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	m_Sprite = LoadAssetRef<ASprite>(_File);
}
