#include "pch.h"
#include "PlayerJumpState.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

PlayerJumpState::PlayerJumpState()
{
	m_FlipbookIndex = PLAYER_STATE::JUMP;
}

PlayerJumpState::~PlayerJumpState()
{
}

void PlayerJumpState::SaveToLevelFile(FILE* _File)
{
}

void PlayerJumpState::LoadFromLevelFile(FILE* _File)
{
}

void PlayerJumpState::Begin()
{

}

void PlayerJumpState::Tick()
{
	if (KEY_PRESSED(KEY::SPACE))
	{
		// Player의 데이터를 받아와 Position 변경
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vRot = m_PlayerData->GetTargetObject()->Transform()->GetRelativeRot();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		vPos.y += DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeRot(vRot);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}
}

void PlayerJumpState::FinalTick()
{
}

PLAYER_STATE PlayerJumpState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

unique_ptr<PlayerState> PlayerJumpState::Clone() const
{
	return make_unique<PlayerJumpState>(*this);
}
