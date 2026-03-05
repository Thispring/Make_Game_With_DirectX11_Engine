#include "pch.h"
#include "CBillboardRender.h"
#include "AssetMgr.h"

CBillboardRender::CBillboardRender()
	: CRenderComponent(COMPONENT_TYPE::BILLBOARD_RENDER)
{
}

CBillboardRender::~CBillboardRender()
{
}

void CBillboardRender::FinalTick()
{
	// 기능 구현 X
}

void CBillboardRender::Render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	GetMaterial()->SetScalar(VEC2_0, m_BillboardScale);

	GetMaterial()->Binding();

	GetMesh()->Render();

	GetMaterial()->Clear();
}

void CBillboardRender::CreateMaterial()
{
	// Mesh 설정
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"SquareMesh"));
	
	// 만들어진 재질 찾기
	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(L"BillboardMtrl");

	// 찾는 재질이 없다면 생성
	if (pMtrl == nullptr)
	{
		// 재질 생성
		pMtrl = new AMaterial;
		pMtrl->SetName(L"BillboardMtrl");

		// 쉐이더를 찾아서 재질에 세팅
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(L"BillboardShader");
		
		// 찾는 쉐이더가 없으면 생성해서
		// 에셋매니저에 등록
		if (pShader == nullptr)
		{
			pShader = new AGraphicShader;
			pShader->SetName(L"BillboardShader");
			pShader->CreateVertexShader(L"Shader\\billboard.fx", "VS_Billboard");
			pShader->CreatePixelShader(L"Shader\\billboard.fx", "PS_Billboard");
			pShader->SetBSType(BS_TYPE::DEFAULT);
			pShader->SetRSType(RS_TYPE::CULL_NONE);
			AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());
		}

		// 찾았거나 생성한 쉐이더를 재질에 설정하고
		// 재질을 에셋매니저에 등록
		pMtrl->SetShader(pShader);
		// 재질의 도메인 등록
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
		AssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(pMtrl);
}
