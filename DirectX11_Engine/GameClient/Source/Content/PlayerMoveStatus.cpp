#include "pch.h"
#include "PlayerMoveStatus.h"
#include "PlayerStatus.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

PlayerMoveStatus::PlayerMoveStatus()
{
	m_FlipbookIndex = PLAYER_FLIPBOOK::MOVE;
}

PlayerMoveStatus::~PlayerMoveStatus()
{
}

void PlayerMoveStatus::SaveToLevelFile(FILE* _File)
{
}

void PlayerMoveStatus::LoadFromLevelFile(FILE* _File)
{
}

void PlayerMoveStatus::Begin()
{
}

void PlayerMoveStatus::Tick()
{
	if (KEY_PRESSED(KEY::A))
	{
		// Player의 데이터를 받아와 Position 변경
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vRot = m_PlayerData->GetTargetObject()->Transform()->GetRelativeRot();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		// Scale x축 음수화
		if (vScale.x >= 0) 
			vScale.x *= -1.f;

		vPos.x -= DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeRot(vRot);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}
	
	if (KEY_PRESSED(KEY::D))
	{
		// Player의 데이터를 받아와 Position 변경
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vRot = m_PlayerData->GetTargetObject()->Transform()->GetRelativeRot();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		// Scale x축 양수화
		if (vScale.x <= 0)
			vScale.x *= -1.f;

		vPos.x += DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeRot(vRot);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}

}

void PlayerMoveStatus::FinalTick()
{

}

int PlayerMoveStatus::GetFlipbookIndex()
{
	return (int)m_FlipbookIndex;
}

unique_ptr<PlayerStatus> PlayerMoveStatus::Clone() const
{
	return make_unique<PlayerMoveStatus>(*this);
}
	