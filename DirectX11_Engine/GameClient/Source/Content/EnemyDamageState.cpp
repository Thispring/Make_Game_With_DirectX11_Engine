#include "pch.h"
#include "EnemyDamageState.h"
#include "Source\Scripts\CEnemyStateManager.h"
#include "Source\Scripts\CPlayerStateManager.h"

#include "GameMgr.h"
#include "TimeMgr.h"

#include "SoundMgr.h"

#pragma region EnemyDamageState
EnemyDamageState::EnemyDamageState(Ptr<CEnemyData> _Data)
    : EnemyState(_Data)
{
}

EnemyDamageState::~EnemyDamageState()
{
}
#pragma endregion


#pragma region EnemyHitState
EnemyHitState::EnemyHitState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyHitState::~EnemyHitState()
{
}


void EnemyHitState::OnBegin()
{
    // Play hit sound according to enemy type
    ENEMY_TYPE type = m_EnemyData->GetEnemyType();
    if (type == ENEMY_TYPE::DEMON)
    {
        SoundMgr::GetInst()->PlaySFX(L"DEMON_Hit");
    }
    else if (type == ENEMY_TYPE::SKULL)
    {
        SoundMgr::GetInst()->PlaySFX(L"SKULL_Hit");
    }
    else if (type == ENEMY_TYPE::FLYING)
    {
        SoundMgr::GetInst()->PlaySFX(L"FLYING_Hit");
    }
    else if (type == ENEMY_TYPE::FLOWER)
    {
        SoundMgr::GetInst()->PlaySFX(L"FLOWER_Hit");
    }
}

void EnemyHitState::OnTick()
{
	// HIT Flipbook 재생이 끝나면 IDLE로 복귀
	if (m_EnemyData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
	{
        // SKULL 타입 분기 처리
        // SKULL 타입이 EnergyBlast와 충돌 시, 무적 상태로 전환

		Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
		pMgr->SetCurStatus(pMgr->GetStatusByCommonState(ENEMY_STATE::IDLE));
		pMgr->ChangeState();
	}
}

void EnemyHitState::OnFinalTick()
{
    //Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetOwner()->GetScript<CEnemyStateManager>();
    //pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)GetEnemyStateToParam(m_EnemyData->GetEnemyType(), ENEMY_STATE::IDLE)));
    //pMgr->ChangeState();
}


void EnemyHitState::SaveToLevelFile(FILE* _File)
{
}

void EnemyHitState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyHitState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion


#pragma region EnemyDeadState
EnemyDeadState::EnemyDeadState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyDeadState::~EnemyDeadState()
{
}

void EnemyDeadState::OnBegin()
{

}

void EnemyDeadState::OnTick()
{
    // Dead Flipbook 재생이 끝나면 비활성화
    if (m_EnemyData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
    {
        //m_EnemyData->GetTargetObject()->GetChild(ENEMY_EYES)->SetIsActive(false);
        m_EnemyData->GetTargetObject()->SetIsActive(false);
    }
}

void EnemyDeadState::OnFinalTick()
{
}


void EnemyDeadState::SaveToLevelFile(FILE* _File)
{
}

void EnemyDeadState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyDeadState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion


#pragma region EnemyGhostSkullState
EnemyGhostSkullState::EnemyGhostSkullState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyGhostSkullState::~EnemyGhostSkullState()
{
}

void EnemyGhostSkullState::OnBegin()
{
    // 타입을 GHOST_SKULL로 변경 -> 부모 클래스 중력 적용 제외하기 위함
    m_EnemyData->SetEnemyType(ENEMY_TYPE::GHOST_SKULL);

    // Start skull ghost SFX looping without overlap
    SoundMgr::GetInst()->PlaySFX(L"SKULL_Ghost", 0, false);
}

void EnemyGhostSkullState::OnTick()
{
    // Flipbook 재생이 끝나면 GhostSkullMoveState로 전환
    if (m_EnemyData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
    {
        Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
        pMgr->SetCurStatus(pMgr->GetStatusByCommonState(ENEMY_STATE::GHOST_SKULL_MOVE));
        pMgr->ChangeState();
    }
}

void EnemyGhostSkullState::OnFinalTick()
{

}


void EnemyGhostSkullState::SaveToLevelFile(FILE* _File)
{
}

void EnemyGhostSkullState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyGhostSkullState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion


#pragma region EnemyGhostSkullMoveState
EnemyGhostSkullMoveState::EnemyGhostSkullMoveState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyGhostSkullMoveState::~EnemyGhostSkullMoveState()
{
}

void EnemyGhostSkullMoveState::OnBegin()
{

}

void EnemyGhostSkullMoveState::OnTick()
{
    // 플레이어 nullptr 방어
    Ptr<GameObject> pPlayer = GameMgr::GetInst()->GetPlayer();
    if (pPlayer == nullptr)
        return;

    Vec3  pos = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
    Vec3  scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
    float speed = m_EnemyData->GetSpeed() * 2.f;
    int   dir = m_EnemyData->GetDirection();

    Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();

    // 거리 계산 (단일 계산, 이후 이동에도 재사용)
    float dx = vPlayerPos.x - pos.x;
    float dy = vPlayerPos.y - pos.y;
    float len = sqrtf(dx * dx + dy * dy);

    // 거의 겹치는 상태: 적 스케일 절반 이내일 때 데미지 적용
    float attackRange = fabsf(scale.x) * 0.5f;
    if (len <= attackRange)
    {
        float frameDamage = m_EnemyData->GetDamage() * 2.f * DT;
        pPlayer->GetScript<CPlayerStateManager>()->TakeDamage(frameDamage);
    }

    // X축 방향에 따른 스프라이트 좌우 반전
    int newDir = (vPlayerPos.x > pos.x) ? 1 : -1;
    if (newDir != dir)
    {
        scale.x *= -1.f;
        dir = newDir;
    }

    // X, Y 양 축 방향 벡터로 이동 (근접하지 않은 경우만)
    if (len > 1.f)
    {
        pos.x += (dx / len) * speed * DT;
        pos.y += (dy / len) * speed * DT;
    }

    // 적용
    m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
    m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
    m_EnemyData->SetDirection(dir);
}

void EnemyGhostSkullMoveState::OnFinalTick()
{
    m_EnemyData->SetEnemyType(ENEMY_TYPE::SKULL);
    // Ensure ghost SFX stopped when this state ends
    SoundMgr::GetInst()->StopSFX(L"SKULL_Ghost");
}


void EnemyGhostSkullMoveState::SaveToLevelFile(FILE* _File)
{
}

void EnemyGhostSkullMoveState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyGhostSkullMoveState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion