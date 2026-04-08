#include "pch.h"
#include "EnemyChaseState.h"

#include "GameMgr.h"
#include "TimeMgr.h"

#include "Source\Scripts\CPlayerData.h"

EnemyChaseState::EnemyChaseState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{
}

EnemyChaseState::~EnemyChaseState()
{
}

void EnemyChaseState::OnBegin()
{
	// FLYING 타입은 추격 시작 시 현재 위치와 회전값을 원점으로 저장
	if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLYING)
	{
		Vec3 pos = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
		Vec3 rot = m_EnemyData->GetTargetObject()->Transform()->GetRelativeRot();
		m_EnemyData->SetOriginPos(pos);
		m_EnemyData->SetOriginRot(rot);
	}
}

void EnemyChaseState::OnTick()
{
	Vec3  pos   = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
	float speed = m_EnemyData->GetSpeed();

	// 플레이어 위치 가져오기
	Vec3 vPlayerPos = GameMgr::GetInst()->GetPlayer()->Transform()->GetRelativePos();

	if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLYING)
	{
		// FLYING: x, y 양축으로 플레이어를 향해 이동
		// 1. Pos X축 음수 방향 일때 본인의 Scale에 음수 연산을 해주어야함
		// 2. 추격이 끝나고 본인 위치 y축으로 이동하는 기능, Idle에 제대로 구현되어 있는지 확인

		Vec3  scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
		int   dir = m_EnemyData->GetDirection();

		// 거리 계산 (단일 계산, 이후 이동에도 재사용)
		float dx = vPlayerPos.x - pos.x;
		float dy = vPlayerPos.y - pos.y;
		float len = sqrtf(dx * dx + dy * dy);

		// X축 방향에 따른 스프라이트 좌우 반전
		int newDir = (vPlayerPos.x > pos.x) ? 1 : -1;
		if (newDir != dir)
		{
			scale.x *= -1.f;
			dir = newDir;
		}

		pos.x += (dx / len) * speed * DT * 2.f;
		pos.y += (dy / len) * speed * DT * 2.f;


		// 적용
		m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
		m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
		m_EnemyData->SetDirection(dir);
	}
	else
	{
		Vec3  scale  = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
		int   dir    = m_EnemyData->GetDirection();
		float offSet = m_EnemyData->GetOffset();

		// 1. 플레이어가 오른쪽이면 +1, 왼쪽이면 -1
		int newDir = (vPlayerPos.x > pos.x) ? 1 : -1;

		// 2. 방향이 바뀌면 스프라이트 좌우 반전
		if (newDir != dir)
		{
			scale.x *= -1.f;
			dir = newDir;
		}

		// Boss 타입 추적 speed 증가
		if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::BOSS)
			speed *= 4.f;

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
}

void EnemyChaseState::OnFinalTick()
{

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

