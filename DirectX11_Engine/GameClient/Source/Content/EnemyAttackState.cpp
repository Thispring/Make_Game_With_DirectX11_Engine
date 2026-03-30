#include "pch.h"
#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState()
{
}

EnemyAttackState::EnemyAttackState(const wstring& ownerName)
	: EnemyState(ownerName)
{
}

EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::Begin()
{
}

void EnemyAttackState::Tick()
{
}

void EnemyAttackState::FinalTick()
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

