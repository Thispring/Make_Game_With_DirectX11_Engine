#include "pch.h"
#include "CEnemyStatus.h"
#include "GameObject.h"
#include "TimeMgr.h"


CEnemyStatus::CEnemyStatus()
	: CScript(SCRIPT_TYPE::ENEMYSTATUS)
	, m_IsFalling(true)
{
}

CEnemyStatus::~CEnemyStatus()
{
}

void CEnemyStatus::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyStatus::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;
}

void CEnemyStatus::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyStatus::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyStatus::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyStatus::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyStatus::EndOverlap);
}

void CEnemyStatus::Tick()
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

void CEnemyStatus::SaveToLevelFile(FILE* _File)
{
}

void CEnemyStatus::LoadFromLevelFile(FILE* _File)
{
}
