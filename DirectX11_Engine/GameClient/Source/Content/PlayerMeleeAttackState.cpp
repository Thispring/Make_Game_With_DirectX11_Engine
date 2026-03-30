#include "pch.h"
#include "PlayerMeleeAttackState.h"
#include "KeyMgr.h"
#include "Source\Scripts\CPlayerStateManager.h"

#pragma region PlayerMeleeAttackState
PlayerMeleeAttackState::PlayerMeleeAttackState()
{
}

PlayerMeleeAttackState::~PlayerMeleeAttackState()
{
}

void PlayerMeleeAttackState::Begin()
{
}

void PlayerMeleeAttackState::Tick()
{
}

void PlayerMeleeAttackState::FinalTick()
{
}

PLAYER_STATE PlayerMeleeAttackState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerMeleeAttackState::SaveToLevelFile(FILE* _File)
{
}

void PlayerMeleeAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerMeleeAttackState::Clone() const
{
	return unique_ptr<PlayerState>();
}

#pragma endregion


#pragma region PlayerPunchState
PlayerPunchState::PlayerPunchState()
{
	m_FlipbookIndex = PLAYER_STATE::PUNCH;
}

PlayerPunchState::~PlayerPunchState()
{
}

void PlayerPunchState::Begin()
{
}

void PlayerPunchState::Tick()
{
	// Z Key를 눌렀을때, CPlayerData에서 공격 관련
	// bool 변수를 true로 변경, Z Key를 누르고 있지 않다면
	// bool 변수를 false로 변경
	
	//if (KEY_RELEASED(KEY::Z))
	//{
	//	bool isAttck = false;
	//	m_PlayerData->SetIsAttack(isAttck);
	//}

	bool isAttck = true;
	m_PlayerData->SetIsAttack(isAttck);
	//if (KEY_PRESSED(KEY::Z))
	//{
	//	bool isAttck = true;
	//	m_PlayerData->SetIsAttack(isAttck);
	//}
}

void PlayerPunchState::FinalTick()
{
	bool isAttck = false;
	m_PlayerData->SetIsAttack(isAttck);

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
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
PlayerMiddleKickState::PlayerMiddleKickState()
{
	m_FlipbookIndex = PLAYER_STATE::MIDDLE_KICK;
}

PlayerMiddleKickState::~PlayerMiddleKickState()
{
}

void PlayerMiddleKickState::Begin()
{
}

void PlayerMiddleKickState::Tick()
{
	bool isAttck = true;
	m_PlayerData->SetIsAttack(isAttck);
}

void PlayerMiddleKickState::FinalTick()
{
	bool isAttck = false;
	m_PlayerData->SetIsAttack(isAttck);

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
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
PlayerHighKickState::PlayerHighKickState()
{
	m_FlipbookIndex = PLAYER_STATE::HIGH_KICK;
}

PlayerHighKickState::~PlayerHighKickState()
{
}

void PlayerHighKickState::Begin()
{
}

void PlayerHighKickState::Tick()
{
	bool isAttck = true;
	m_PlayerData->SetIsAttack(isAttck);
}

void PlayerHighKickState::FinalTick()
{
	bool isAttck = false;
	m_PlayerData->SetIsAttack(isAttck);

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
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
PlayerLowKickState::PlayerLowKickState()
{
	m_FlipbookIndex = PLAYER_STATE::LOW_KICK;
}

PlayerLowKickState::~PlayerLowKickState()
{
}

void PlayerLowKickState::Begin()
{
}

void PlayerLowKickState::Tick()
{
	bool isAttck = true;
	m_PlayerData->SetIsAttack(isAttck);
}

void PlayerLowKickState::FinalTick()
{
	bool isAttck = false;
	m_PlayerData->SetIsAttack(isAttck);

	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
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