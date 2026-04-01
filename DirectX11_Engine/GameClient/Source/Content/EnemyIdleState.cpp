#include "pch.h"
#include "EnemyIdleState.h"

#include "Source\Scripts\CEnemyStateManager.h"

EnemyIdleState::EnemyIdleState(Ptr<CEnemyData> _Data)
    : EnemyState(_Data)
{
    // _Data로 초기화된 m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
    ENEMY_TYPE type = (ENEMY_TYPE)0;
    if (m_EnemyData != nullptr)
        type = m_EnemyData->GetEnemyType();

    switch (type)
    {
    case ENEMY_TYPE::DEMON:
        m_FlipbookIndex = ENEMY_STATE::DEMON_IDLE;
        break;
    case ENEMY_TYPE::SKULL:
        m_FlipbookIndex = ENEMY_STATE::SKULL_IDLE;
        break;
    case ENEMY_TYPE::FLYING:
        m_FlipbookIndex = ENEMY_STATE::FLYING_IDLE;
        break;
    case ENEMY_TYPE::FLOWER:
        m_FlipbookIndex = ENEMY_STATE::FLOWER_IDLE;
        break;
    case ENEMY_TYPE::BOSS:
        m_FlipbookIndex = ENEMY_STATE::BOSS_IDLE;
        break;
    }
}

EnemyIdleState::~EnemyIdleState()
{
}


void EnemyIdleState::OnBegin()
{
    
}

void EnemyIdleState::OnTick()
{
    // 첫 스폰 이후 2초가 지났다면 Patrol 상태로 변경
    // 2.5f보다 작아야한다는 조건문을 추가해, 최초 소환만 보장
    float spawnTime = m_EnemyData->GettimeSinceSpawn();
    if (spawnTime >= 2.f && spawnTime <= 2.5f)
    {
        Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
        pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_COMMON_STATE::PATROL));
        pMgr->ChangeState();
        return;
    }


    // 다시 Idle로 돌아왔을 때, 1초 후 Patrol 상태로 변경
    if (m_EnemyData->GettimeInState() >= 1.f)
    {
        Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
        pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_COMMON_STATE::PATROL));
        pMgr->ChangeState();
    }
}

void EnemyIdleState::OnFinalTick()
{
}

ENEMY_STATE EnemyIdleState::GetFlipbookIndex()
{
    return m_FlipbookIndex;
}

void EnemyIdleState::SaveToLevelFile(FILE* _File)
{
}

void EnemyIdleState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyIdleState::Clone() const
{
    return unique_ptr<EnemyState>();
}
