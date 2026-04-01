#include "pch.h"
#include "PlayerMeleeAttackState.h"
#include "KeyMgr.h"
#include "Source\Scripts\CPlayerStateManager.h"

#pragma region PlayerMeleeAttackState
PlayerMeleeAttackState::PlayerMeleeAttackState(Ptr<CPlayerData> _Data)
	: PlayerState(_Data)
{
}

PlayerMeleeAttackState::~PlayerMeleeAttackState()
{
}

#pragma endregion


#pragma region PlayerPunchState
PlayerPunchState::PlayerPunchState(Ptr<CPlayerData> _Data)
	: PlayerMeleeAttackState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::PUNCH;
}

PlayerPunchState::~PlayerPunchState()
{
}

void PlayerPunchState::Begin()
{
	m_PlayerData->SetIsAttack();
}

void PlayerPunchState::Tick()
{

}

void PlayerPunchState::FinalTick()
{
	m_PlayerData->OffIsAttack();
	m_PlayerData->GetTargetObject()->GetChild(PLAYER_PUNCH_ANCHOR)->Collider2D()->SetEnabled(false);

	// Idle 상태로 변경
    Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
}

PLAYER_STATE PlayerPunchState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerPunchState::SaveToLevelFile(FILE* _File)
{
}

void PlayerPunchState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerPunchState::Clone() const
{
	return unique_ptr<PlayerState>();
}

#pragma endregion


#pragma region PlayerMiddleKickState
PlayerMiddleKickState::PlayerMiddleKickState(Ptr<CPlayerData> _Data)
	: PlayerMeleeAttackState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::MIDDLE_KICK;
}

PlayerMiddleKickState::~PlayerMiddleKickState()
{
}

void PlayerMiddleKickState::Begin()
{
	m_PlayerData->SetIsAttack();
}

void PlayerMiddleKickState::Tick()
{
}

void PlayerMiddleKickState::FinalTick()
{
	m_PlayerData->OffIsAttack();
	m_PlayerData->GetTargetObject()->GetChild(PLAYER_KICK_ANCHOR)->Collider2D()->SetEnabled(false);

	// Idle 상태로 변경
    Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
}

PLAYER_STATE PlayerMiddleKickState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerMiddleKickState::SaveToLevelFile(FILE* _File)
{
}

void PlayerMiddleKickState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerMiddleKickState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion


#pragma region PlayerHighKickState
PlayerHighKickState::PlayerHighKickState(Ptr<CPlayerData> _Data)
	: PlayerMeleeAttackState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::HIGH_KICK;
}

PlayerHighKickState::~PlayerHighKickState()
{
}

void PlayerHighKickState::Begin()
{
	m_PlayerData->SetIsAttack();
}

void PlayerHighKickState::Tick()
{
}

void PlayerHighKickState::FinalTick()
{
	m_PlayerData->OffIsAttack();

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
    pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
}

PLAYER_STATE PlayerHighKickState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerHighKickState::SaveToLevelFile(FILE* _File)
{
}

void PlayerHighKickState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerHighKickState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion


#pragma region PlayerLowKickState
PlayerLowKickState::PlayerLowKickState(Ptr<CPlayerData> _Data)
	: PlayerMeleeAttackState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::LOW_KICK;
}

PlayerLowKickState::~PlayerLowKickState()
{
}

void PlayerLowKickState::Begin()
{
	m_PlayerData->SetIsAttack();
}

void PlayerLowKickState::Tick()
{
}

void PlayerLowKickState::FinalTick()
{
	m_PlayerData->OffIsAttack();

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
    pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
}

PLAYER_STATE PlayerLowKickState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerLowKickState::SaveToLevelFile(FILE* _File)
{
}

void PlayerLowKickState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerLowKickState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion