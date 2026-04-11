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
    // ApplyGravity()에서는 양수 속도를 위쪽(상승)으로 처리하므로 양수로 설정합니다.
    m_PlayerData->SetVelocityY(fabsf(m_PlayerData->GetJumpVelocity()));
    // 점프 시작 시 공중 상태로 만들되, 물리 업데이트와 점프-착지 판정이
    // 일관되게 동작하도록 ground contact를 초기화하고 약간 위로 밀어줍니다.
    m_PlayerData->SetIsFalling(true);
    m_PlayerData->SetIsJumping(true);

    // ground contact 카운트 초기화: BeginOverlap/EndOverlap에서 바닥 판정에
    // 의해 감소/증가 되므로 여기서는 0으로 초기화하여 공중 상태를 보장
    m_PlayerData->SetGroundContactCount(0);

    // 콜라이더가 아직 바닥과 겹쳐있을 수 있으므로 작은 너징으로 충돌 해제
    Vec3 pos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
    pos.y += 1.0f; // small nudge to escape ground overlap
    m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(pos);
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
