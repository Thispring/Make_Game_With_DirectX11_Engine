#include "pch.h"
#include "EnemyJumpState.h"

EnemyJumpState::EnemyJumpState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
}

EnemyJumpState::~EnemyJumpState()
{
}

void EnemyJumpState::OnBegin()
{
}

void EnemyJumpState::OnTick()
{
}

void EnemyJumpState::OnFinalTick()
{
}

ENEMY_STATE EnemyJumpState::GetFlipbookIndex()
{
	return ENEMY_STATE();
}

void EnemyJumpState::SaveToLevelFile(FILE* _File)
{
}

void EnemyJumpState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyJumpState::Clone() const
{
	return unique_ptr<EnemyState>();
}

