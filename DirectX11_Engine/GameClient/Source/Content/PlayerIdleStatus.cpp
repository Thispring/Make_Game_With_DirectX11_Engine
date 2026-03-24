#include "pch.h"
#include "PlayerIdleStatus.h"
#include "TimeMgr.h"


PlayerIdleStatus::PlayerIdleStatus()
{
	m_FlipbookIndex = PLAYER_FLIPBOOK::IDLE;
}

PlayerIdleStatus::~PlayerIdleStatus()
{
}

void PlayerIdleStatus::SaveToLevelFile(FILE* _File)
{
}

void PlayerIdleStatus::LoadFromLevelFile(FILE* _File)
{
}

void PlayerIdleStatus::Begin()
{

}

void PlayerIdleStatus::Tick()
{
	if (m_PlayerData->GetIsFalling() == true)
	{
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		vPos.y -= DT * 120.f;
		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
	}
}

void PlayerIdleStatus::FinalTick()
{
}

int PlayerIdleStatus::GetFlipbookIndex()
{
	return (int)m_FlipbookIndex;
}

unique_ptr<PlayerStatus> PlayerIdleStatus::Clone() const
{
	return make_unique<PlayerIdleStatus>(*this);
}
