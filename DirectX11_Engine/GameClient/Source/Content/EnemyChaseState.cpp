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
		float dx   = vPlayerPos.x - pos.x;
		float dy   = vPlayerPos.y - pos.y;
		float dist = sqrtf(dx * dx + dy * dy);

		if (dist > 5.f)
		{
			float invDist = 1.f / dist;
			pos.x += dx * invDist * speed * DT * 2.f;
			pos.y += dy * invDist * speed * DT * 2.f;
		}

		// 수평 방향이 바뀌었을 때만 scale.x 반전
		Vec3 scale  = m_EnemyData->GetTargetObject()->Transform()->GetRelativeScale();
		int  dir    = m_EnemyData->GetDirection();
		int  newDir = (dx > 0.f) ? 1 : -1;
		if (newDir != dir)
		{
			scale.x *= -1.f;
			m_EnemyData->SetDirection(newDir);
			m_EnemyData->GetTargetObject()->Transform()->SetRelativeScale(scale);
		}

		m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(pos);
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

