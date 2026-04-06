#include "pch.h"
#include "CSpriteRender.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
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
	// 아직 Sprite 설정이 안되어있다면 return
	if (m_Sprite == nullptr)
		return;

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
	wstring MeshName = L"SquareMesh";
	wstring MtrlName = L"Material\\SpriteMtrl.mtrl";
	wstring ShaderName = L"SpriteShader";
	wstring FilePath = L"Shader\\sprite.fx";
	string VS = "VS_Tile";
	string PS = "PS_Tile";

	// RectMesh 설정
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(MeshName));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(MtrlName);

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = NEW AMaterial;
		pMtrl->SetName(MtrlName);

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(ShaderName);

		// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다
		if (nullptr == pShader)
		{
			pShader = NEW AGraphicShader;
			pShader->SetName(ShaderName);
			pShader->CreateVertexShader(FilePath, VS);
			pShader->CreatePixelShader(FilePath, PS);
			pShader->SetBSType(BS_TYPE::DEFAULT);
			pShader->SetRSType(RS_TYPE::CULL_NONE);
			AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());
		}

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		// 재질의 도메인 등록
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
		// 생성한 재질 파일 저장
		pMtrl->Save(CONTENT_PATH + pMtrl->GetKey());
	}

	SetMaterial(pMtrl);
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

Ptr<ASprite> CSpriteRender::CreateDynamicSprite()
{
	// 동적 스프라이트 생성은 Level이 Play 상태일 때만 사용
	assert(LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState());

	if (m_DynamicSprite != nullptr)
	{
		m_Sprite = m_DynamicSprite;
		return m_DynamicSprite;
	}
	else
	{
		m_Sprite = m_DynamicSprite = (ASprite*)m_Sprite->Clone();
		return m_DynamicSprite;
	}
}
