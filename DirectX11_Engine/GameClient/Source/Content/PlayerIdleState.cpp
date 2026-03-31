#include "pch.h"
#include "PlayerIdleState.h"
#include "TimeMgr.h"
#include "TaskMgr.h"


PlayerIdleState::PlayerIdleState(Ptr<CPlayerData> _Data)
	: PlayerState(_Data)
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
	// 비활성화는 다음 프레임으로 연기하여 충돌 콜백(EndOverlap 등)이 보장되도록 함
	TaskMgr::GetInst()->SetActiveDeferred(m_PlayerData->GetTargetObject()->GetChild(1).Get(), false);
	TaskMgr::GetInst()->SetActiveDeferred(m_PlayerData->GetTargetObject()->GetChild(2).Get(), false);
}

void PlayerIdleState::Tick()
{
	PlayerState::ApplyGravity();
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
