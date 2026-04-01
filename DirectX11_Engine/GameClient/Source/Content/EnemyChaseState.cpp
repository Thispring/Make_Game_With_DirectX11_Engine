#include "pch.h"
#include "EnemyChaseState.h"

EnemyChaseState::EnemyChaseState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
	// _Data로 초기화된 m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
	ENEMY_TYPE type = (ENEMY_TYPE)0;
	if (m_EnemyData != nullptr)
		type = m_EnemyData->GetEnemyType();

	// Chase는 이동 로직을 수행하므로 MOVE로 초기화
	switch (type)
	{
	case ENEMY_TYPE::DEMON:
		m_FlipbookIndex = ENEMY_STATE::DEMON_MOVE;
		break;
	case ENEMY_TYPE::SKULL:
		m_FlipbookIndex = ENEMY_STATE::SKULL_MOVE;
		break;
	case ENEMY_TYPE::FLYING:
		m_FlipbookIndex = ENEMY_STATE::FLYING_MOVE;
		break;
	case ENEMY_TYPE::FLOWER:
		m_FlipbookIndex = ENEMY_STATE::FLOWER_MOVE;
		break;
	case ENEMY_TYPE::BOSS:
		m_FlipbookIndex = ENEMY_STATE::BOSS_MOVE;
		break;
	}
}

EnemyChaseState::~EnemyChaseState()
{
}

void EnemyChaseState::OnBegin()
{
}

void EnemyChaseState::OnTick()
{
}

void EnemyChaseState::OnFinalTick()
{
}

ENEMY_STATE EnemyChaseState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void EnemyChaseState::SaveToLevelFile(FILE* _File)
{
}

void EnemyChaseState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyChaseState::Clone() const
{
	return unique_ptr<EnemyState>();
}

