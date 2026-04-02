#include "pch.h"
#include "EnemyAttackState.h"

#include "Source\Scripts\CPlayerStateManager.h"

#include "GameMgr.h"
#include "TimeMgr.h"

EnemyAttackState::EnemyAttackState(Ptr<CEnemyData> _Data)
	: EnemyState(_Data)
{

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

