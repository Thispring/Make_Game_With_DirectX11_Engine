#include "pch.h"
#include "EnemyIdleState.h"

EnemyIdleState::EnemyIdleState()
{
   // 부모 기본 생성자 호출은 멤버 이니셜라이저에서 자동으로 이루어집니다.
   // m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
   ENEMY_TYPE type = ENEMY_TYPE::DEMON;
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

EnemyIdleState::EnemyIdleState(const std::wstring& ownerName)
    : EnemyState(ownerName)
{
    // ownerName으로 초기화된 m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
    ENEMY_TYPE type = ENEMY_TYPE::DEMON;
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

void EnemyIdleState::Begin()
{
}

void EnemyIdleState::Tick()
{
    EnemyState::ApplyGravity();
}

void EnemyIdleState::FinalTick()
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

