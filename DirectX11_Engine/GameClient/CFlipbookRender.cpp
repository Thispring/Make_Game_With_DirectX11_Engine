#include "pch.h"
#include "CFlipbookRender.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
#include "TimeMgr.h"

#include "GameObject.h"

CFlipbookRender::CFlipbookRender()
	: CRenderComponent(COMPONENT_TYPE::FLIPBOOK_RENDER)
	, m_FPS(0.f)
	, m_AccTime(0.f)
	, m_CurSprite(0)
	, m_Finish(false)
	, m_RepeatCount(0)
	, m_IsStop(false)
	, m_CurFlipbook(0)
{
}

CFlipbookRender::~CFlipbookRender()
{
}

bool CFlipbookRender::CheckFinish()
{
	if (m_Finish)
	{
		if (0 < m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			--m_RepeatCount;
			return false;
		}
		else if (-1 == m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			return false;
		}
		else
		{
			return true;
		}
	}
}

void CFlipbookRender::DeleteFlipbook(int _Idx)
{
	m_vecFlipbook.erase(m_vecFlipbook.begin() + _Idx);
	m_CurFlipbook = 0;	// 재생중인 인덱스를 0으로 설정
}

void CFlipbookRender::FinalTick()
{
	// Level이 Play상태 일때만 재생하도록 설정
	/*if (LevelMgr::GetInst()->GetLevelState() != LEVEL_STATE::PLAY)
		return;*/

	if (GetIsStop())
		return;

	if (CheckFinish())
		return;


	float fLmit = 1.f / m_FPS;
	m_AccTime += DT;

	if (fLmit < m_AccTime)
	{
		m_AccTime -= fLmit;
		++m_CurSprite;

		if (m_vecFlipbook[m_CurFlipbook]->GetSpriteCount() <= m_CurSprite)
		{
			m_Finish = true;
			--m_CurSprite;
		}
	}
}

void CFlipbookRender::Render()
{
	// 설정된 Flipbook이 없다면 return
	if (m_vecFlipbook.empty())
		return;

	Ptr<AFlipbook> pCurFlipbook = m_vecFlipbook[m_CurFlipbook];
	Ptr<ASprite> pCurSprite = pCurFlipbook->GetSprite(m_CurSprite);

	GetMaterial()->SetTexture(TEX_0, pCurSprite->GetAtlas());
	GetMaterial()->SetScalar(VEC2_0, pCurSprite->GetLeftTopUV());
	GetMaterial()->SetScalar(VEC2_1, pCurSprite->GetSliceUV());
	GetMaterial()->SetScalar(VEC2_2, pCurSprite->GetBackgroundUV());
	GetMaterial()->SetScalar(VEC2_3, pCurSprite->GetOffsetUV());

	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CFlipbookRender::CreateMaterial()
{
	// 플레이어와 Enemy는 같은 FlipBook Sprite를 공유하고 있고,
	// 두 FlipBook의 색상을 변경하기 위해, 분기를 나누어서
	// 각자 다른 Shader 파일로 바인딩하기

	// NOTE(26-02-11): 조건 분기를 나누어도, 가리키는 pMtrl이 동일하기 때문에
	// 같은 Material로 랜더링 되어, 의도한 동작이 되지 않음
	// Material를 새로 생성해주거나, FlipbookRender에 멤버를 추가하여
	// 멤버 생성 시, 조건에 따라 값을 나누고, 그 값을 Material 바인딩 시 활용하는 방식 고려

	wstring MeshName = L"SquareMesh";
	wstring MtrlName = L"FlipbookMtrl";
	wstring ShaderName = L"FlipbookShader";


	// GameObject의 이름에 따라 문자열에 이름 추가하여
	// 새로운 Material과 Shader 생성, 
	// 임시방편이므로 추후 더 좋은 방법이 있다면 변경하기

	// 찾으려는 pShader가 없다면 이 함수에서 새로 생성하고,
	// 여기서 생성한 pShader를 둘다 공유하기 때문에, 조건분기를 해서
	// 값을 다르게 줘도 덮여 씌여지고 있음

	wstring ObjName = GetOwner()->GetName();
	wstring FilePath = L"Shader\\flipbook.fx";;


	string VS = "VS_Flipbook";
	string PS = "PS_Flipbook";

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

void CFlipbookRender::SaveToLevelFile(FILE* _File)
{
	// 부모인 CRenderComponent에 구현되어 있는 함수 사용
	CRenderComponent::SaveToLevelFile(_File);

	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);

	for (const auto& Flipbook : m_vecFlipbook)
	{
		SaveAssetRef(_File, Flipbook.Get());
	}

	fwrite(&m_CurFlipbook, sizeof(int), 1, _File);
	fwrite(&m_CurSprite, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(int), 1, _File);
	fwrite(&m_RepeatCount, sizeof(int), 1, _File);
}

void CFlipbookRender::LoadFromLevelFile(FILE* _File)
{
	// 부모인 CRenderComponent에 구현되어 있는 함수 사용
	CRenderComponent::LoadFromLevelFile(_File);

	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		m_vecFlipbook.push_back(LoadAssetRef<AFlipbook>(_File));
	}

	fread(&m_CurFlipbook, sizeof(int), 1, _File);
	fread(&m_CurSprite, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(int), 1, _File);
	fread(&m_RepeatCount, sizeof(int), 1, _File);
}
