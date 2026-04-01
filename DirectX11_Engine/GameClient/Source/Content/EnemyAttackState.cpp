#include "pch.h"
#include "EnemyAttackState.h"


EnemyAttackState::EnemyAttackState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
}

EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::OnBegin()
{
}

void EnemyAttackState::OnTick()
{
}

void EnemyAttackState::OnFinalTick()
{
}

ENEMY_STATE EnemyAttackState::GetFlipbookIndex()
{
	return ENEMY_STATE();
}

void EnemyAttackState::SaveToLevelFile(FILE* _File)
{
}

void EnemyAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyAttackState::Clone() const
{
	return unique_ptr<EnemyState>();
}

