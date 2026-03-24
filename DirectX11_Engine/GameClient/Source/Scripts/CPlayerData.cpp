#include "pch.h"
#include "CPlayerData.h"


CPlayerData::CPlayerData()
	: CScript(SCRIPT_TYPE::PLAYERDATA)
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_Speed(1.f)

	, m_DeathCount(0)

	, m_IsDead(false)
	, m_IsFalling(true)
{
}

CPlayerData::~CPlayerData()
{
}

void CPlayerData::Begin()
{
}

void CPlayerData::Tick()
{
}

void CPlayerData::SaveToLevelFile(FILE* _File)
{
}

void CPlayerData::LoadFromLevelFile(FILE* _File)
{
}
