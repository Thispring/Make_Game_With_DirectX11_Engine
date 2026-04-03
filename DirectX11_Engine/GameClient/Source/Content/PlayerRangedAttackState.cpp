#include "pch.h"
#include "PlayerRangedAttackState.h"
#include "Source\Scripts\CPlayerStateManager.h"

#pragma region PlayerRangedAttackState
PlayerRangedAttackState::PlayerRangedAttackState(Ptr<CPlayerData> _Data)
	: PlayerState(_Data)
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
PlayerChargeState::PlayerChargeState(Ptr<CPlayerData> _Data)
	: PlayerRangedAttackState(_Data)
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


#pragma region PlayerReleaseState

#pragma endregion


#pragma region PlayerEnergyBlastShotState
PlayerEnergyBlastShotState::PlayerEnergyBlastShotState(Ptr<CPlayerData> _Data)
	: PlayerRangedAttackState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::ENERGYBLAST_SHOT;
}

PlayerEnergyBlastShotState::~PlayerEnergyBlastShotState()
{
}

void PlayerEnergyBlastShotState::Begin()
{
	m_PlayerData->SetIsAttack();
}

void PlayerEnergyBlastShotState::Tick()
{
	PlayerState::ApplyGravity();
}

void PlayerEnergyBlastShotState::FinalTick()
{
	// 정리(cleanup)만 담당 — Idle 전환은 CPlayerAnimator::Tick()에서 처리
	m_PlayerData->OffIsAttack();
}

PLAYER_STATE PlayerEnergyBlastShotState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerEnergyBlastShotState::SaveToLevelFile(FILE* _File)
{
}

void PlayerEnergyBlastShotState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerEnergyBlastShotState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion
