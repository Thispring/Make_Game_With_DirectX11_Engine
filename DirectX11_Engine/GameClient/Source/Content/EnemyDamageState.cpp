#include "pch.h"
#include "EnemyDamageState.h"
#include "Source\Scripts\CEnemyStateManager.h"

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
    ENEMY_TYPE type = (ENEMY_TYPE)0;
    if (m_EnemyData != nullptr)
        type = m_EnemyData->GetEnemyType();

    switch (type)
    {
    case ENEMY_TYPE::DEMON:
        m_FlipbookIndex = ENEMY_STATE::DEMON_HIT;
        break;
    case ENEMY_TYPE::SKULL:
        m_FlipbookIndex = ENEMY_STATE::SKULL_HIT;
        break;
    case ENEMY_TYPE::FLYING:
        m_FlipbookIndex = ENEMY_STATE::FLYING_HIT;
        break;
    case ENEMY_TYPE::FLOWER:
        m_FlipbookIndex = ENEMY_STATE::FLOWER_HIT;
        break;
    case ENEMY_TYPE::BOSS:
        m_FlipbookIndex = ENEMY_STATE::BOSS_HIT;
        break;
    }
}

EnemyHitState::~EnemyHitState()
{
}


void EnemyHitState::Begin()
{

}

void EnemyHitState::Tick()
{
}

void EnemyHitState::FinalTick()
{
    //Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetOwner()->GetScript<CEnemyStateManager>();
    //pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)GetEnemyStateToParam(m_EnemyData->GetEnemyType(), ENEMY_COMMON_STATE::IDLE)));
    //pMgr->ChangeState();
}

ENEMY_STATE EnemyHitState::GetFlipbookIndex()
{
    return m_FlipbookIndex;
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
    ENEMY_TYPE type = (ENEMY_TYPE)0;
    if (m_EnemyData != nullptr)
        type = m_EnemyData->GetEnemyType();

    switch (type)
    {
    case ENEMY_TYPE::DEMON:
        m_FlipbookIndex = ENEMY_STATE::DEMON_DEAD;
        break;
    case ENEMY_TYPE::SKULL:
        m_FlipbookIndex = ENEMY_STATE::SKULL_DEAD;
        break;
    case ENEMY_TYPE::FLYING:
        m_FlipbookIndex = ENEMY_STATE::FLYING_DEAD;
        break;
    case ENEMY_TYPE::FLOWER:
        m_FlipbookIndex = ENEMY_STATE::FLOWER_DEAD;
        break;
    case ENEMY_TYPE::BOSS:
        m_FlipbookIndex = ENEMY_STATE::BOSS_DEAD;
        break;
    }
}

EnemyDeadState::~EnemyDeadState()
{
}

void EnemyDeadState::Begin()
{
}

void EnemyDeadState::Tick()
{
}

void EnemyDeadState::FinalTick()
{
}

ENEMY_STATE EnemyDeadState::GetFlipbookIndex()
{
    return m_FlipbookIndex;
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