#include "pch.h"
#include "PlayerMoveState.h"

#include "PlayerState.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"

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

	Vec3 vNormal  = m_PlayerData->GetGroundNormal();
	Vec3 vTangent = Vec3(vNormal.y, -vNormal.x, 0.f);

	if (KEY_PRESSED(KEY::LEFT) && !m_PlayerData->GetIsBlockedLeft())
	{
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		if (vScale.x >= 0) 
			vScale.x *= -1.f;

		vPos.x -= vTangent.x * DT * m_PlayerData->GetSpeed();
		vPos.y -= vTangent.y * DT * m_PlayerData->GetSpeed();

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
		m_PlayerData->GetTargetObject()->Transform()->SetRelativeScale(vScale);
	}

	if (KEY_PRESSED(KEY::RIGHT) && !m_PlayerData->GetIsBlockedRight())
	{
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 vScale = m_PlayerData->GetTargetObject()->Transform()->GetRelativeScale();

		if (vScale.x <= 0)
			vScale.x *= -1.f;

		vPos.x += vTangent.x * DT * m_PlayerData->GetSpeed();
		vPos.y += vTangent.y * DT * m_PlayerData->GetSpeed();

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