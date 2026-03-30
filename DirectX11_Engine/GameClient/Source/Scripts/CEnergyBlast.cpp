#include "pch.h"
#include "CEnergyBlast.h"
#include "LevelMgr.h"
#include "TimeMgr.h"

CEnergyBlast::CEnergyBlast()
	: CScript(SCRIPT_TYPE::ENERGYBLAST)
	, m_Dir {}
	, m_TravelTime(0.f)
	, m_Speed(100.f)
	, m_Damage(5.f)
	, m_VelocityX(0.f)

	, m_IsCollided(false)
	, m_IsDestroy(false)
{
}

CEnergyBlast::~CEnergyBlast()
{
}

void CEnergyBlast::SetUp(Vec3 _Dir)
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
}

void CEnergyBlast::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CEnergyBlast::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CEnergyBlast::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CEnergyBlast::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CEnergyBlast::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnergyBlast::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnergyBlast::EndOverlap);
}

void CEnergyBlast::Tick()
{
	// 생성된 방향으로 3초간 가속도를 붙여 발사됩니다.
	// 이전 프로젝트 미사일 발사 코드 참조
	// 3초가 지났거나, Enemy와 부딪혔다면 삭제요청
	Vec3 vPos = Transform()->GetRelativePos();

	// 방향 정보를 매개변수로 받고, 그 방향만큼 이동을 곱함
	vPos += m_Dir * 200.f * DT;

	Transform()->SetRelativePos(vPos);

	m_TravelTime += DT;

	// 3초이상 지났다면 삭제요청
	if (m_TravelTime >= 3.f)
	{
		Destroy();
		return;
	}
}

void CEnergyBlast::SaveToLevelFile(FILE* _File)
{
}

void CEnergyBlast::LoadFromLevelFile(FILE* _File)
{
}
