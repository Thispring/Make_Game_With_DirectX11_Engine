#include "pch.h"
#include "EnemyMoveState.h"


EnemyMoveState::EnemyMoveState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
}

EnemyMoveState::~EnemyMoveState()
{
}

void EnemyMoveState::OnBegin()
{
}

void EnemyMoveState::OnTick()
{
}

void EnemyMoveState::OnFinalTick()
{
}

ENEMY_STATE EnemyMoveState::GetFlipbookIndex()
{
	return ENEMY_STATE();
}

void EnemyMoveState::SaveToLevelFile(FILE* _File)
{
}

void EnemyMoveState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyMoveState::Clone() const
{
	return unique_ptr<EnemyState>();
}

