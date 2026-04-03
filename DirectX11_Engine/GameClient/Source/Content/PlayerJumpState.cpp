#include "pch.h"
#include "PlayerJumpState.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Source\Scripts\CPlayerStateManager.h"

PlayerJumpState::PlayerJumpState(Ptr<CPlayerData> _Data)
    : PlayerState(_Data)
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
    // 점프 시작 시 한 번만 초기 속도 부여
    // ApplyGravity()의 부호 규약에 맞추어 음수(상승 방향)로 설정합니다.
    // m_JumpVelocity는 '절대값'으로 보관되는 설계라면 여기에서 부호를 반전.
    m_PlayerData->SetVelocityY(fabsf(m_PlayerData->GetJumpVelocity()));
    m_PlayerData->SetIsFalling(true);
    m_PlayerData->SetIsJumping(true);
}

void PlayerJumpState::Tick()
{
    // 물리 통합(중력 적용 및 위치 업데이트)은 공통 로직에 위임
    ApplyGravity();

    // 필요한 추가 공중 입력 처리(예: 좌우 이동, 점프 유지 등)는 여기에 추가
}

void PlayerJumpState::FinalTick()
{
    // 정리(cleanup)만 담당 — Idle 전환은 CPlayerAnimator::Tick()에서 처리
    m_PlayerData->SetIsJumping(false);
}

PLAYER_STATE PlayerJumpState::GetFlipbookIndex()
{
    return m_FlipbookIndex;
}

unique_ptr<PlayerState> PlayerJumpState::Clone() const
{
    return make_unique<PlayerJumpState>(*this);
}
