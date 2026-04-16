#include "pch.h"
#include "CSavePoint.h"

#include "GameObject.h"

#include "GameMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CSavePoint::CSavePoint()
	: CScript(SCRIPT_TYPE::SAVEPOINT)
	, m_InitialPos{}
	, m_OriginPos{}
	, m_BobTime(0.f)
	, m_bTriggered(false)
{
}

CSavePoint::~CSavePoint()
{
}

void CSavePoint::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// Player와 충돌했을 때, GameMgr을 통해 SavePoint 이벤트를 호출시켜
		// Player의 세이브 위치 정보를 갱신합니다.
		GameMgr::GetInst()->RegisterPlayerSave(GetOwner()->Transform()->GetRelativePos());

		// 사운드 재생
		SoundMgr::GetInst()->PlaySFX(L"SavePoint");

		// 사라지는 Flipbook 재생
		// 재생이 끝나면 기능이 다했으므로 비활성화 or 삭제 요청을 합니다.
		GetOwner()->FlipbookRender()->Play(1, 30, 0);
		m_bTriggered = true;
	}
}

void CSavePoint::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CSavePoint::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CSavePoint::Init()
{
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_InitialPos, L"InitialPos", true, 0.f);
}

void CSavePoint::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CSavePoint::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CSavePoint::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CSavePoint::EndOverlap);

	// 동적 재질 생성
	GetOwner()->FlipbookRender()->CreateDynamicMaterial();

	// 첫 생성 컬러 변화 보장
	Ptr<AMaterial> pMtrl = GetOwner()->FlipbookRender()->GetMaterial();
	pMtrl->SetScalar(VEC4_0, ColorConvertIntToVec4(252.f, 152.f, 3.f));

	// Idle Flipbook 재생
	GetOwner()->FlipbookRender()->Play(0, 5, -1);

	// 위치 초기화
	GetOwner()->Transform()->SetRelativePos(m_InitialPos);

	// 위아래 반복 움직임의 기준 위치 저장
	m_OriginPos = GetOwner()->Transform()->GetRelativePos();
}

void CSavePoint::Tick()
{
	// 소멸 애니메이션 재생이 끝나면 오브젝트를 비활성화
	if (m_bTriggered && GetOwner()->FlipbookRender()->GetFinish())
	{
		SetActiveDeferred(GetOwner(), false);
		return;
	}

	// DT를 누적 → sinf 입력값이 매 프레임 증가하여 올바르게 진동
	m_BobTime += DT;

	Vec3 vPos = m_OriginPos;
	vPos.y += sinf(m_BobTime * BOB_SPEED) * BOB_AMPLITUDE;
	GetOwner()->Transform()->SetRelativePos(vPos);

	// CreateDynamicMaterial() 호출 후 GetMaterial()은 동적 복사본을 반환
	Ptr<AMaterial> pMtrl = GetOwner()->FlipbookRender()->GetMaterial();

	// SavePoint Flipbook의 Vec4_0번째 파라미터로 컬러값을 전달
	pMtrl->SetScalar(VEC4_0, ColorConvertIntToVec4(252.f, 152.f, 3.f));
}

void CSavePoint::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_InitialPos, sizeof(Vec3), 1, _File);
}

void CSavePoint::LoadFromLevelFile(FILE* _File)
{
	fread(&m_InitialPos, sizeof(Vec3), 1, _File);
}
