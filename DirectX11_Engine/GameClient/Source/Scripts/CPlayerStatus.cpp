#include "pch.h"
#include "CPlayerStatus.h"
#include "GameObject.h"
#include "TimeMgr.h"

CPlayerStatus::CPlayerStatus()
	: CScript(SCRIPT_TYPE::PLAYERSTATUS)
	, m_FullHP(10)
	, m_CurHP(m_FullHP)
	, m_Damage(1)
	, m_Speed(5)
	, m_DeathCount(0)
	, m_IsDead(false)
	, m_IsFalling(true)
{

}

CPlayerStatus::~CPlayerStatus()
{
}

void CPlayerStatus::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerStatus::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;
}

void CPlayerStatus::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerStatus::Init()
{
	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
}

void CPlayerStatus::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerStatus::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerStatus::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerStatus::EndOverlap);
}

void CPlayerStatus::Tick()
{
	if (m_IsFalling)
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		Vec3 vDown = -vUp;

		vPos += vDown * 250.f * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

}

void CPlayerStatus::SaveToLevelFile(FILE* _File)
{
}

void CPlayerStatus::LoadFromLevelFile(FILE* _File)
{
}

