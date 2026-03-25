#include "pch.h"
#include "PlayerJumpState.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Source\Scripts\CPlayerStateManager.h"

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
    // 위 방향 속도를 순간적으로 부여 (y축이 위가 +라면 양수, 아래가 +라면 음수)
    // 여기서는 일반적인 2D 좌표계(아래가 +Y)를 기준으로 하강이 +, 상승이 -라고 가정합니다.
    m_PlayerData->SetVelocityY(m_PlayerData->GetJumpVelocity());
    m_PlayerData->SetIsFalling(true);

    float VelY = m_PlayerData->GetVelocityY();

    // 중력 적용: 속도가 점점 아래(+) 방향으로 커짐
    VelY += m_PlayerData->GetGravity() * DT * 10.f;;
    m_PlayerData->SetVelocityY(VelY);

    // 위치 업데이트
    Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
    vPos.y += VelY * DT * 10.f;
    m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);

}

void PlayerJumpState::FinalTick()
{
	// Idle 상태로 변경
	Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetOwner()->GetScript<CPlayerStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusVec((int)PLAYER_STATE::IDLE));

	// 점프 후 추락 상태로 전환
	m_PlayerData->SetIsFalling(true);
}

PLAYER_STATE PlayerJumpState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

unique_ptr<PlayerState> PlayerJumpState::Clone() const
{
	return make_unique<PlayerJumpState>(*this);
}
