#include "pch.h"
#include "EnemyAttackState.h"

#include "Source\Scripts\CPlayerStateManager.h"

#include "GameMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"

#include "APrefab.h"

#pragma region EnemyAttackState(근접)
EnemyAttackState::EnemyAttackState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{

}

EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::OnBegin()
{
	m_bInterrupted = false;

	// 이벤트 구독: TakeDamage 호출 시 공격 즉시 중단
	m_EnemyData->SubscribeOnTakeDamage([this]()
	{
		m_bInterrupted = true;
	});
}

void EnemyAttackState::OnTick()
{
	// 피격 이벤트 수신 시 공격 로직 건너뜀
	if (m_bInterrupted)
		return;

	// 초당 데미지를 주고 싶다면
	// 데미지 계산 Tick 프레임과 DT 고려
	float dps = m_EnemyData->GetDamage() * 2.f;
	float frameDamage = dps * DT;

	// 매 프레임 즉시 반영
	GameMgr::GetInst()->GetPlayer()->GetScript<CPlayerStateManager>()->TakeDamage(frameDamage);
}

void EnemyAttackState::OnFinalTick()
{
	// 상태 종료 시 구독 해제 (다른 상태에서 오염 방지)
	m_EnemyData->UnsubscribeOnTakeDamage();
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
#pragma endregion


#pragma region EnemyRangedAttackState (원거리 공격)
EnemyRangedAttackState::EnemyRangedAttackState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
	, m_fireTime(0.f)
{

}

EnemyRangedAttackState::~EnemyRangedAttackState()
{
}

void EnemyRangedAttackState::OnBegin()
{
	// 첫 공격 보장
	m_fireTime += 3.f;
}

void EnemyRangedAttackState::OnTick()
{
	// EnemyData Prefab 멤버를 자신의 방향(스케일)에 맞게 발사
	// 3초마다 함수 실행
	m_fireTime += DT;

	if (m_fireTime >= 3.f)
	{
		m_EnemyData->CreateProjectile();
		m_fireTime = 0;
		return;
	}
}

void EnemyRangedAttackState::OnFinalTick()
{
}

void EnemyRangedAttackState::SaveToLevelFile(FILE* _File)
{
}

void EnemyRangedAttackState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyRangedAttackState::Clone() const
{
	return unique_ptr<EnemyState>();
}
#pragma endregion