#include "pch.h"
#include "PlayerMoveState.h"
#include "PlayerState.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

PlayerMoveState::PlayerMoveState(Ptr<CPlayerData> _Data)
	: PlayerState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::WALK;
}

PlayerMoveState::~PlayerMoveState()
{
}

void PlayerMoveState::SaveToLevelFile(FILE* _File)
{
}

void PlayerMoveState::LoadFromLevelFile(FILE* _File)
{
}

void PlayerMoveState::Begin()
{
}

void PlayerMoveState::Tick()
{
	PlayerState::ApplyGravity();

	if (KEY_PRESSED(KEY::LEFT))
	{
		// Player의 데이터를 받아와 Position 변경
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		// Scale x축 음수화
		if (vScale.x >= 0) 
			vScale.x *= -1.f;

		vPos.x -= DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}
	
	if (KEY_PRESSED(KEY::RIGHT))
	{
		// Player의 데이터를 받아와 Position 변경
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		// Scale x축 양수화
		if (vScale.x <= 0)
			vScale.x *= -1.f;

		vPos.x += DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}

}

void PlayerMoveState::FinalTick()
{

}

PLAYER_STATE PlayerMoveState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

unique_ptr<PlayerState> PlayerMoveState::Clone() const
{
	return make_unique<PlayerMoveState>(*this);
}
	