#include "pch.h"
#include "CFlowerProjectile.h"

#include "GameObject.h"

#include "CPlayerStateManager.h"
#include "TimeMgr.h"

CFlowerProjectile::CFlowerProjectile()
	: CScript(SCRIPT_TYPE::FLOWERPROJECTILE)
	, m_Dir{}
	, m_TravelTime(0.f)
	, m_Speed(100.f)
	, m_Damage(5.f)
	, m_VelocityX(0.f)

	, m_IsCollided(false)
	, m_IsDestroy(false)
{
}

CFlowerProjectile::~CFlowerProjectile()
{
}

void CFlowerProjectile::SetUp(Vec3 _Dir)
{
	// 매개변수로 받은 방향정보로 세팅
	m_Dir = _Dir;
	// _Dir의 x가 음수라면 본인의 Scale x를 음수로 바꾸기
	if (_Dir.x < 0)
	{
		Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
		vScale.x *= -1.f;
		GetOwner()->Transform()->SetRelativeScale(vScale);
	}

	// 시점 문제를 해결하기 위해 z축을 100으로 고정
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	vPos.z = 100.f;
	GetOwner()->Transform()->SetRelativePos(vPos);

	// Flipbook 재생
	GetOwner()->FlipbookRender()->Play(0, 10, -1);
}

bool CFlowerProjectile::DestroyProjectile()
{
	// 함수에서 사라지는 Flipbook만 Play 요청하고
	// Tick에서 재생이 멈추었는지 검사
	if (m_IsDestroy)
		return false;

	m_IsDestroy = true;
	GetOwner()->FlipbookRender()->Play(1, 10, 1);

	return false;
}

void CFlowerProjectile::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// void CPlayerStateManager::TakeDamage(float _Damage) 호출
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// 여기에서 _OtherCollider한테 데미지값을 전달해야함
		_OtherCollider->GetOwner()->GetScript<CPlayerStateManager>()->TakeDamage(GetDamage());
	}
}

void CFlowerProjectile::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// Player Layer에서만 작동하게 조건문 실행
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		DestroyProjectile();
	}
}

void CFlowerProjectile::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CFlowerProjectile::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CFlowerProjectile::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CFlowerProjectile::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CFlowerProjectile::EndOverlap);
}

void CFlowerProjectile::Tick()
{
	if (m_IsDestroy)
	{
		// Flipbook이 끝났는지 체크
		if (GetOwner()->FlipbookRender()->GetFinish())
		{
			Destroy(); // 실제 삭제
		}
		// 아직 재생이 끝나지 않았으면 return
		return;
	}

	// 생성된 방향으로 3초간 가속도를 붙여 발사됩니다.
	// 이전 프로젝트 미사일 발사 코드 참조
	// 3초가 지났거나, Enemy와 부딪혔다면 삭제요청
	Vec3 vPos = Transform()->GetRelativePos();

	// 방향 정보를 매개변수로 받고, 그 방향만큼 이동을 곱함
	vPos += m_Dir * 200.f * DT;

	Transform()->SetRelativePos(vPos);

	m_TravelTime += DT;

	// 시간에 따른 삭제요청
	if (m_TravelTime >= 3.f)
	{
		DestroyProjectile();
		return;
	}
}

void CFlowerProjectile::SaveToLevelFile(FILE* _File)
{
}

void CFlowerProjectile::LoadFromLevelFile(FILE* _File)
{
}

