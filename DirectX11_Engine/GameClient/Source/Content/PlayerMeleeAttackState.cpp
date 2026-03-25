#include "pch.h"
#include "PlayerMeleeAttackState.h"
#include "KeyMgr.h"

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

	if (KEY_TAP(KEY::Z))
	{
		bool isAttck = true;
		m_PlayerData->SetIsAttack(isAttck);
	}
}

void PlayerPunchState::FinalTick()
{
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


#pragma region KickState
PlayerKickState::PlayerKickState()
{
}

PlayerKickState::~PlayerKickState()
{
}

void PlayerKickState::Begin()
{
}

void PlayerKickState::Tick()
{
}

void PlayerKickState::FinalTick()
{
}

PLAYER_STATE PlayerKickState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void PlayerKickState::SaveToLevelFile(FILE* _File)
{
}

void PlayerKickState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerKickState::Clone() const
{
	return unique_ptr<PlayerState>();
}
#pragma endregion
