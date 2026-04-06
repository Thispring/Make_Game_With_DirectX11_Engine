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

    if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLYING)
    {
        // FLYING: Origin으로 서서히 복귀 후 offset 초기화 -> PATROL 전환
        Vec3  pos       = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
        Vec3  rot       = m_EnemyData->GetTargetObject()->Transform()->GetRelativeRot();
        Vec3  originPos = m_EnemyData->GetOriginPos();
        Vec3  originRot = m_EnemyData->GetOriginRot();
        float speed     = m_EnemyData->GetSpeed();

        float dx   = originPos.x - pos.x;
        float dy   = originPos.y - pos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist > 2.f)
        {
            float invDist = 1.f / dist;
            pos.x += dx * invDist * speed * DT;
            pos.y += dy * invDist * speed * DT;
            rot.z  = atan2f(dy, dx);

            m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
            m_EnemyData->GetTargetObject()->Transform()->SetRelativeRot(rot);
        }
        else
        {
            // Origin 도달: 스냅, offset 초기화, PATROL 전환
            pos   = originPos;
            rot.z = originRot.z;
            m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
            m_EnemyData->GetTargetObject()->Transform()->SetRelativeRot(rot);

            m_EnemyData->SetOffset(0.f);

            Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
            pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::PATROL));
            pMgr->ChangeState();
        }
    }
    else
    {
        // FLOWER 타입은 IDLE 유지
        if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
        {
            Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
            pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
            pMgr->ChangeState();
        }

        // 다시 Idle로 돌아왔을 때, 1초 후 Patrol 상태로 변경
        if (m_EnemyData->GetTimeInState() >= 0.25f)
        {
            Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
            pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::PATROL));
            pMgr->ChangeState();
        }
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
