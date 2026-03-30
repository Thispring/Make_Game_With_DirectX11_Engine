#include "pch.h"
#include "EnemyJumpState.h"

EnemyJumpState::EnemyJumpState()
{
}

EnemyJumpState::EnemyJumpState(const std::wstring& ownerName)
	: EnemyState(ownerName)
{
}

EnemyJumpState::~EnemyJumpState()
{
}

void EnemyJumpState::Begin()
{
}

void EnemyJumpState::Tick()
{
}

void EnemyJumpState::FinalTick()
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

