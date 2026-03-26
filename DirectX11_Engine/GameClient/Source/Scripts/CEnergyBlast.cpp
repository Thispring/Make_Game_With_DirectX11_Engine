#include "pch.h"
#include "CEnergyBlast.h"
#include "LevelMgr.h"

CEnergyBlast::CEnergyBlast()
	: CScript(SCRIPT_TYPE::ENERGYBLAST)
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

}

void CEnergyBlast::SaveToLevelFile(FILE* _File)
{
}

void CEnergyBlast::LoadFromLevelFile(FILE* _File)
{
}
