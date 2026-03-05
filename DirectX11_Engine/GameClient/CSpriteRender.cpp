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

	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CSpriteRender::CreateMaterial()
{
	wstring MeshName = L"SquareMesh";
	wstring MtrlName = L"SpriteMtrl";
	wstring ShaderName = L"SpriteShader";

	wstring ObjName = GetOwner()->GetName();
	wstring FilePath = L"";

	// Player와 Enemy가 같은 sprite를 쓰고있어서 분기처리
	if (ObjName == L"Player")
		FilePath = L"Shader\\sprite.fx";
	else if (ObjName == L"Enemy")
		FilePath = L"Shader\\enemysprite.fx";
	else
		FilePath = L"Shader\\sprite.fx";

	string VS = "VS_Sprite";
	string PS = "PS_Sprite";

	// RectMesh 설정
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(MeshName));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(MtrlName);

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = new AMaterial;
		pMtrl->SetName(MtrlName);

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(ShaderName);

		// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다
		if (nullptr == pShader)
		{
			pShader = new AGraphicShader;
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
	}

	SetMaterial(pMtrl);
}
