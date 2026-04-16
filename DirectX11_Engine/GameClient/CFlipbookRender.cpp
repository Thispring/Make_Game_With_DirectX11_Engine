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
	, m_PrevFlipbook(0)
	, m_UseAlphaBlending(false)
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

	// 명시적 false 반환
	// 이전은 특정 조건에만 false 반환
	return false;
}

void CFlipbookRender::DeleteFlipbook(int _Idx)
{
	m_vecFlipbook.erase(m_vecFlipbook.begin() + _Idx);
	m_CurFlipbook = 0;	// 재생중인 인덱스를 0으로 설정
}

void CFlipbookRender::Play(int _FlipbookIdx, float _FPS, int _RepeatCount)
{
	// 동일한 Flipbook에 대해 매번 Play를 호출하면
	// m_AccTime을 리셋하여 애니메이션이 진행되지 않는 문제가 발생.
	// 따라서 동일 Flipbook인 경우에는 누적 시간을 유지하도록 변경.
	bool IsNewFlipbook = (_FlipbookIdx != m_PrevFlipbook);

	if (IsNewFlipbook)
	{
		// _FlipbookIdx이 -1이 들어왔다면, 강제로 Idle 재생하는것이므로
		// 인덱스를 0으로 설정
		if (_FlipbookIdx == -1)
			_FlipbookIdx = 0;
		// 새로운 Flipbook이면 처음부터 재생
		m_CurSprite = 0;
		m_AccTime = 0.f;
	}

	// 현재, 이전 상태 Flipbook 인덱스 저장
	m_CurFlipbook = m_PrevFlipbook = _FlipbookIdx;
	m_RepeatCount = _RepeatCount;
	m_FPS = _FPS;

	// 만약 강제 초기화가 필요하면(옵션) 여기서 처리
}

void CFlipbookRender::EditorPlay(int _FlipbookIdx, float _FPS, int _RepeatCount)
{
	// ImGui 전용 Play 함수,
	// 새로 들어온 유뮤와 상관없이 값을 초기화합니다.

	m_AccTime = 0.f;
	m_CurSprite = 0;
	// 현재, 이전 상태 Flipbook 인덱스 저장
	m_CurFlipbook = m_PrevFlipbook = _FlipbookIdx;
	m_RepeatCount = _RepeatCount;
	m_FPS = _FPS;
}

void CFlipbookRender::FinalTick()
{
	// Editor Mode에서 재생을 원하면 DT를  
	// E_DT로 변경하면 됩니다.
	if (GetIsStop())
		return;

	if (CheckFinish())
		return;

	// FPS가 0일 경우 분모가 0이 됨
	if (m_FPS <= 0.f)
		return;

	float fLmit = 1.f / m_FPS;
	m_AccTime += DT;

	if (fLmit < m_AccTime)
	{
		m_AccTime -= fLmit;
		++m_CurSprite;

		if (m_vecFlipbook[m_CurFlipbook]->GetSpriteCount() <= (UINT)m_CurSprite)
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

	//=========================================================================
	// NOTE(26-04-16):
	// BS_TYPE이 DEFAULT로 설정되있어서 투명도 옵션이 적용이 안되었음
	// Flipbook을 사용하려면 아래 조건문 처럼 따로 BS_TYPE을 ALPHABLEND로 설정해야함
	//=========================================================================
	if (m_UseAlphaBlending)
		GetMaterial()->GetShader()->SetBSType(BS_TYPE::ALPHABLEND);
	else
		GetMaterial()->GetShader()->SetBSType(BS_TYPE::DEFAULT);

	GetMaterial()->SetTexture(TEX_0, pCurSprite->GetAtlas());
	GetMaterial()->SetScalar(VEC2_0, pCurSprite->GetLeftTopUV());
	GetMaterial()->SetScalar(VEC2_1, pCurSprite->GetSliceUV());
	GetMaterial()->SetScalar(VEC2_2, pCurSprite->GetBackgroundUV());
	GetMaterial()->SetScalar(VEC2_3, pCurSprite->GetOffsetUV());

	// SavePoint int값 전달
	GetMaterial()->SetScalar(INT_0, 1);

	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CFlipbookRender::CreateMaterial()
{
	wstring MeshName = L"SquareMesh";
	wstring MtrlName = L"Material\\FlipbookMtrl.mtrl";
	wstring ShaderName = L"FlipbookShader";

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
		// 생성한 재질을 파일로 저장
		pMtrl->Save(CONTENT_PATH + pMtrl->GetKey());
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
	fwrite(&m_FPS, sizeof(float), 1, _File);
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

	/***********************************************************************
	* NOTE(26-04-04):
	* 한 오브젝트가 여러 Flipbook을 가지고 있을 때,
	* 아래 멤버를 저장 및 불러올 시, 불러오는 과정에서 
	* 다른 Flipbook의 인덱스 참조 오류가 발생할 수 있습니다.
	* Flipbook 재생은 각자 오브젝트 Begin 시점에서 필요한 값으로 초기화합니다.
	***********************************************************************/
	fread(&m_CurFlipbook, sizeof(int), 1, _File);
	fread(&m_CurSprite, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_RepeatCount, sizeof(int), 1, _File);

	// 불러온 뒤 0으로 초기화
	m_CurFlipbook = 0;
	m_CurSprite = 0;
	m_FPS = 0;
	m_RepeatCount = 0;
}
