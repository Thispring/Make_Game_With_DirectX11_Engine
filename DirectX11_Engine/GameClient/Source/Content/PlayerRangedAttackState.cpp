#include "pch.h"
#include "PlayerRangedAttackState.h"
#include "Source\Scripts\CPlayerStateManager.h"

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


#pragma region PlayerReleaseState

#pragma endregion


#pragma region PlayerEnergyBlastShotState
PlayerEnergyBlastShotState::PlayerEnergyBlastShotState()
{
	m_FlipbookIndex = PLAYER_STATE::ENERGYBLAST_SHOT;
}

PlayerEnergyBlastShotState::~PlayerEnergyBlastShotState()
{
}

void PlayerEnergyBlastShotState::Begin()
{
}

void PlayerEnergyBlastShotState::Tick()
{
}

void PlayerEnergyBlastShotState::FinalTick()
{
	bool isAttck = false;
	m_PlayerData->SetIsAttack(isAttck);

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
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
