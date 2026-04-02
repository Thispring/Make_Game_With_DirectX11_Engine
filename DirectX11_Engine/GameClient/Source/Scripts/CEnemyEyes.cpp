#include "pch.h"
#include "CEnemyEyes.h"

#include "GameObject.h"

#include "CEnemyStateManager.h"

CEnemyEyes::CEnemyEyes()
	: CScript(SCRIPT_TYPE::ENEMYEYES)
{
}

CEnemyEyes::~CEnemyEyes()
{
}

void CEnemyEyes::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_EnemyData->GetIsDead() == true)
	{
		int a = 0;
		return;
	}

	if (_OtherCollider->GetOwner()->GetLayerIdx() == 3)
	{
		// 디버그 들어오는것 확인
		// Chase 상태로 전환
		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();
		pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::CHASE));
		pMgr->ChangeState();
	}
}

void CEnemyEyes::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_EnemyData->GetIsDead() == true)
	{
		int a = 0;
		return;
	}
}

void CEnemyEyes::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_EnemyData->GetIsDead() == true)
	{
		int a = 0;
		return;
	}

	if (_OtherCollider->GetOwner()->GetLayerIdx() == 3)
	{
		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();

		// 현재 CHASE 중일 때만 IDLE로 전환 (다른 상태에서 호출 방지)
		if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::CHASE))
		{
			pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
			pMgr->ChangeState();
		}
	}
}

void CEnemyEyes::Begin()
{
	// NOTE(26-04-02):
	// 함수 호출자가 부모인지 자식인지 꼭 확인
	m_EnemyData = GetOwner()->GetParent()->GetScript<CEnemyData>();

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyEyes::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyEyes::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyEyes::EndOverlap);
}

void CEnemyEyes::Tick()
{
	if (m_EnemyData->GetIsDead() == true)
	{
		GetOwner()->SetIsActive(false);
		return;
	}
}

void CEnemyEyes::SaveToLevelFile(FILE* _File)
{
}

void CEnemyEyes::LoadFromLevelFile(FILE* _File)
{
}
