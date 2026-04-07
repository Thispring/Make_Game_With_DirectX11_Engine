#include "pch.h"
#include "EnemyPatrolState.h"

#include "TimeMgr.h"

EnemyPatrolState::EnemyPatrolState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
	, m_flowerPatrolTime(0.f)
{

}

EnemyPatrolState::~EnemyPatrolState()
{
}

void EnemyPatrolState::OnBegin()
{
	// Chase에서 누적된 offSet를 초기화하여 Patrol 왕복 기준을 현재 위치로 재설정
	m_EnemyData->SetOffset(0.f);
	m_flowerPatrolTime = 0;

	// FLOWER: Patrol 진입 시 scale.x 부호를 기준으로 dir을 즉시 동기화
	// FLOWER 아틈라스는 기본 방향이 왼쪽이므로 scale.x 부호와 시각 방향이 다른 Enemy와 반대
	// scale.x > 0 = 시각적 왼쪽(dir = -1), scale.x < 0 = 시각적 오른쪽(dir = 1)
	if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
	{
		Vec3 scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
		m_EnemyData->SetDirection(scale.x > 0 ? -1 : 1);
	}
}

void EnemyPatrolState::OnTick()
{
	// Flower 타입은 일정 시간 마다
	// 좌우를 바라보는 방법으로 구현
	if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
	{
		m_flowerPatrolTime += DT;

		if (m_flowerPatrolTime >= 2.f)
		{
			// FLOWER 아틈라스는 기본 방향이 왼쪽이므로 scale.x 부호와 시각 방향이 다른 Enemy와 반대
			// scale.x > 0 = 시각적 왼쪽, scale.x < 0 = 시각적 오른쪽
			Vec3 scale = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
			int  dir   = m_EnemyData->GetDirection();

			if (scale.x > 0)	// 현재 시각적으로 왼쪽 → 오른쪽으로 전환
			{
				scale.x *= -1.f;	// 음수 → 시각적 오른쪽
				dir = 1;
			}
			else				// 현재 시각적으로 오른쪽 → 왼쪽으로 전환
			{
				scale.x *= -1.f;	// 양수 → 시각적 왼쪽
				dir = -1;
			}

			m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
			m_EnemyData->SetDirection(dir);

			m_flowerPatrolTime = 0.f;
		}

		return;
	}

	// 현재 위치 가져오기
	Vec3  pos    = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
	Vec3  scale  = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
	int   dir    = m_EnemyData->GetDirection();
	float speed  = m_EnemyData->GetSpeed();
	float offSet = m_EnemyData->GetOffset();

	// 법선에서 접선 계산: 법선을 시계방향 90도 회전 → 오른쪽 이동 방향
	// tangent = (normal.y, -normal.x) — 평지: (0,1)→(1,0), 경사: (-sinθ,cosθ)→(cosθ,sinθ)
	Vec3 vNormal  = m_EnemyData->GetGroundNormal();
	Vec3 vTangent = Vec3(vNormal.y, -vNormal.x, 0.f);

	// 이동량 계산
	float delta = dir * speed * DT;
	pos.x += vTangent.x * delta;	// 경사면 접선 방향으로 이동
	pos.y += vTangent.y * delta;
	offSet += delta;				// 왕복 거리는 이동량 기준 유지

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
