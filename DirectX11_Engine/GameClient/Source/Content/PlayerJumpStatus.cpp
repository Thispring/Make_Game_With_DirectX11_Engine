#include "pch.h"
#include "PlayerJumpStatus.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

PlayerJumpStatus::PlayerJumpStatus()
{
	m_FlipbookIndex = PLAYER_FLIPBOOK::JUMP;
}

PlayerJumpStatus::~PlayerJumpStatus()
{
}

void PlayerJumpStatus::SaveToLevelFile(FILE* _File)
{
}

void PlayerJumpStatus::LoadFromLevelFile(FILE* _File)
{
}

void PlayerJumpStatus::Begin()
{

}

void PlayerJumpStatus::Tick()
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

void PlayerJumpStatus::FinalTick()
{
}

int PlayerJumpStatus::GetFlipbookIndex()
{
	return (int)m_FlipbookIndex;
}

unique_ptr<PlayerStatus> PlayerJumpStatus::Clone() const
{
	return make_unique<PlayerJumpStatus>(*this);
}
