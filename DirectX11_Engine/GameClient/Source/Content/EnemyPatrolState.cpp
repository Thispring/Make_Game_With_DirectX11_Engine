#include "pch.h"
#include "EnemyPatrolState.h"

#include "TimeMgr.h"

EnemyPatrolState::EnemyPatrolState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
	// _Data로 초기화된 m_EnemyData가 존재하면 타입에 따라 FlipbookIndex를 결정
	ENEMY_TYPE type = (ENEMY_TYPE)0;
	if (m_EnemyData != nullptr)
		type = m_EnemyData->GetEnemyType();

	// Patrol은 이동 로직을 수행하므로 MOVE로 초기화
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

EnemyPatrolState::~EnemyPatrolState()
{
}

void EnemyPatrolState::OnBegin()
{

}

void EnemyPatrolState::OnTick()
{
	// 현재 위치 가져오기
	Vec3 pos = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
	Vec3 scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
	int dir = m_EnemyData->GetDirection();
	float speed = m_EnemyData->GetSpeed();
	float offSet = m_EnemyData->GetOffset();

	// 이동량 계산
	float delta = dir * speed * DT;
	pos.x += delta;
	offSet += delta;

	// 한쪽으로 100.f 이상 이동했으면 방향 반전하고 오프셋 정리
	if (dir > 0 && offSet >= 100.f)
	{
		// 초과 보정
		float over = offSet - 100.f;
		pos.x -= over;
		// 반전
		dir = -1;
		offSet = -over; // 다음엔 왼쪽으로 이동, 누적은 음수로 시작
		scale.x *= -1.f;
	}
	else if (dir < 0 && offSet <= -100.f)
	{
		float over = -100.f - offSet; // 양수
		pos.x += over;
		dir = 1;
		offSet = over; // 누적은 양수로 시작
		scale.x *= -1.f;
	}

	// 적용
	m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
	m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
	m_EnemyData->SetDirection(dir);
	m_EnemyData->SetOffset(offSet);
}

void EnemyPatrolState::OnFinalTick()
{
}

ENEMY_STATE EnemyPatrolState::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}

void EnemyPatrolState::SaveToLevelFile(FILE* _File)
{
}

void EnemyPatrolState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyPatrolState::Clone() const
{
	return unique_ptr<EnemyState>();
}
