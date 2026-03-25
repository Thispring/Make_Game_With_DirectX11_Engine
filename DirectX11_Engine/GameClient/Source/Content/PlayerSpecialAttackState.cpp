#include "pch.h"
#include "PlayerSpecialAttackState.h"

#pragma region PlayerSpecialAttackState
PlayerSpecialAttackState::PlayerSpecialAttackState()
{
}

PlayerSpecialAttackState::~PlayerSpecialAttackState()
{
}

void PlayerSpecialAttackState::Begin()
{
}

void PlayerSpecialAttackState::Tick()
{
}

void PlayerSpecialAttackState::FinalTick()
{
}

PLAYER_STATE PlayerSpecialAttackState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerSpecialAttackState::SaveToLevelFile(FILE* _File)
{
}

void PlayerSpecialAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerSpecialAttackState::Clone() const
{
	return unique_ptr<PlayerState>();
}

#pragma endregion
