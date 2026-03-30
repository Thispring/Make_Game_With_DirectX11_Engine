#include "pch.h"
#include "EnemyIdleState.h"

EnemyIdleState::EnemyIdleState()
{
	m_FlipbookIndex = ENEMY_STATE::IDLE;
}

EnemyIdleState::~EnemyIdleState()
{
}

void EnemyIdleState::Begin()
{
}

void EnemyIdleState::Tick()
{
	EnemyState::ApplyGravity();
}

void EnemyIdleState::FinalTick()
{
}

ENEMY_STATE EnemyIdleState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void EnemyIdleState::SaveToLevelFile(FILE* _File)
{
}

void EnemyIdleState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyIdleState::Clone() const
{
	return unique_ptr<EnemyState>();
}

