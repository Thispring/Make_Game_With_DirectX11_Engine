#include "pch.h"
#include "EnemyChaseState.h"

#include "GameMgr.h"
#include "TimeMgr.h"

#include "Source\Scripts\CPlayerData.h"

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
	// 디버깅 들어오는지 확인
	int a = 0;
}

void EnemyChaseState::OnTick()
{
	// 자신의 위치 정보 가져오기
	Vec3 pos   = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
	Vec3 scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
	int  dir   = m_EnemyData->GetDirection();
	float speed  = m_EnemyData->GetSpeed();
	float offSet = m_EnemyData->GetOffset();


	// 플레이어 위치 가져오기
	Vec3 vPlayerPos = GameMgr::GetInst()->GetPlayer()->Transform()->GetRelativePos();

	// 1. 플레이어가 오른쪽이면 +1, 왼쪽이면 -1
	int newDir = (vPlayerPos.x > pos.x) ? 1 : -1;

	// 2. 방향이 바뀌면 스프라이트 좌우 반전
	if (newDir != dir)
	{
		scale.x *= -1.f;
		dir = newDir;
	}

	// 3. 이동량 계산 (X축, 프레임 독립적)
	// 추격 시 더 빠른 스피드 적용
	float delta = dir * speed * DT * 2.f;
	pos.x += delta;
	offSet += delta;

	// 적용
	m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
	m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
	m_EnemyData->SetDirection(dir);
	m_EnemyData->SetOffset(offSet);
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

