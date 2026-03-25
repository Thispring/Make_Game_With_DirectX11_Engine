#include "pch.h"
#include "PlayerIdleState.h"
#include "TimeMgr.h"


PlayerIdleState::PlayerIdleState()
{
	m_FlipbookIndex = PLAYER_STATE::IDLE;
}

PlayerIdleState::~PlayerIdleState()
{
}

void PlayerIdleState::SaveToLevelFile(FILE* _File)
{
}

void PlayerIdleState::LoadFromLevelFile(FILE* _File)
{
}

void PlayerIdleState::Begin()
{

}

void PlayerIdleState::Tick()
{
	if (m_PlayerData->GetIsFalling() == true)
	{
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		vPos.y -= DT * 120.f;
		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
	}
}

void PlayerIdleState::FinalTick()
{
}

PLAYER_STATE PlayerIdleState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

unique_ptr<PlayerState> PlayerIdleState::Clone() const
{
	return make_unique<PlayerIdleState>(*this);
}
