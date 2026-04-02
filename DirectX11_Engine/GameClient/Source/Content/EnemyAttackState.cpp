#include "pch.h"
#include "EnemyAttackState.h"

#include "Source\Scripts\CPlayerStateManager.h"

#include "GameMgr.h"
#include "TimeMgr.h"

EnemyAttackState::EnemyAttackState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
    // _Data로 초기화된 m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
    ENEMY_TYPE type = (ENEMY_TYPE)0;
    if (m_EnemyData != nullptr)
        type = m_EnemyData->GetEnemyType();

    switch (type)
    {
    case ENEMY_TYPE::DEMON:
        m_FlipbookIndex = ENEMY_STATE::DEMON_ATTACK;
        break;
    case ENEMY_TYPE::SKULL:
        m_FlipbookIndex = ENEMY_STATE::SKULL_ATTACK;
        break;
    case ENEMY_TYPE::FLYING:
        m_FlipbookIndex = ENEMY_STATE::FLYING_ATTACK;
        break;
    case ENEMY_TYPE::FLOWER:
        m_FlipbookIndex = ENEMY_STATE::FLOWER_ATTACK;
        break;
    case ENEMY_TYPE::BOSS:
        m_FlipbookIndex = ENEMY_STATE::BOSS_ATTACK;
        break;
    }
}

EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::OnBegin()
{

}

void EnemyAttackState::OnTick()
{
	// 초당 데미지를 주고 싶다면
	// 데미지 계산 Tick 프레임과 DT 고려
	float dps = m_EnemyData->GetDamage() * 2.f;
	float frameDamage = dps * DT;

	// 매 프레임 즉시 반영
	GameMgr::GetInst()->GetPlayer()->GetScript<CPlayerStateManager>()->TakeDamage(frameDamage);
}

void EnemyAttackState::OnFinalTick()
{
}

ENEMY_STATE EnemyAttackState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void EnemyAttackState::SaveToLevelFile(FILE* _File)
{
}

void EnemyAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyAttackState::Clone() const
{
	return unique_ptr<EnemyState>();
}

