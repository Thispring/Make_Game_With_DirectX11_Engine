#include "pch.h"
#include "EnemyIdleState.h"

#include "Source\Scripts\CEnemyStateManager.h"
#include "TimeMgr.h"

EnemyIdleState::EnemyIdleState(Ptr<CEnemyData> _Data)
    : EnemyState(_Data)
{

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
    float spawnTime = m_EnemyData->GetTimeSinceSpawn();
    if (spawnTime >= 2.f && spawnTime <= 2.5f)
    {
        Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
        pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::PATROL));
        pMgr->ChangeState();
        return;
    }

    // 다시 Idle로 돌아왔을 때, 0.25초 후 Patrol 상태로 변경
    if (m_EnemyData->GetTimeInState() >= 0.25f)
    {
        Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
        pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::PATROL));
        pMgr->ChangeState();
    }
}

void EnemyIdleState::OnFinalTick()
{
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
