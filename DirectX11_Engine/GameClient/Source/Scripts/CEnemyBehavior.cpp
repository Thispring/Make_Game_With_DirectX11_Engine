#include "pch.h"
#include "CEnemyBehavior.h"
#include "CEnemyStateManager.h"
#include "LevelMgr.h"

CEnemyBehavior::CEnemyBehavior()
	: CScript(SCRIPT_TYPE::ENEMYBEHAVIOR)
{
}

CEnemyBehavior::~CEnemyBehavior()
{
}

void CEnemyBehavior::Begin()
{
	// Script 주소 참조
	m_Status = GetOwner()->GetScript<CEnemyStateManager>();
}

void CEnemyBehavior::Tick()
{
}

void CEnemyBehavior::SaveToLevelFile(FILE* _File)
{
}

void CEnemyBehavior::LoadFromLevelFile(FILE* _File)
{
}
