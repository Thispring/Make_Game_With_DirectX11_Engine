#include "pch.h"
#include "PlayerRangedAttackState.h"

#pragma region PlayerRangedAttackState
PlayerRangedAttackState::PlayerRangedAttackState()
{
}

PlayerRangedAttackState::~PlayerRangedAttackState()
{
}

void PlayerRangedAttackState::Begin()
{
}

void PlayerRangedAttackState::Tick()
{
}

void PlayerRangedAttackState::FinalTick()
{
}

PLAYER_STATE PlayerRangedAttackState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerRangedAttackState::SaveToLevelFile(FILE* _File)
{
}

void PlayerRangedAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerRangedAttackState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion


#pragma region PlayerChargeState
PlayerChargeState::PlayerChargeState()
{
}

PlayerChargeState::~PlayerChargeState()
{
}

void PlayerChargeState::Begin()
{
}

void PlayerChargeState::Tick()
{
}

void PlayerChargeState::FinalTick()
{
}

PLAYER_STATE PlayerChargeState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerChargeState::SaveToLevelFile(FILE* _File)
{
}

void PlayerChargeState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerChargeState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion
