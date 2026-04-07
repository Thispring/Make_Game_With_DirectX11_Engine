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
		return;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == 3)
	{
		// 만약 GHOST_SKULL 이거나 GHOST_SKULL_MOVE 이라면 추적 X
		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();
		ENEMY_STATE curState = pMgr->GetCurCommonState();
		if (curState == ENEMY_STATE::GHOST_SKULL || curState == ENEMY_STATE::GHOST_SKULL_MOVE)
			return;

		if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
		{
			// FLOWER 타입은 추적모드 전환 X
			// EnemyRangedAttackState 전환
			pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::RANGED_ATTACK));
			pMgr->ChangeState();
			return;
		}

		// 디버그 들어오는것 확인
		// Chase 상태로 전환
		pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::CHASE));
		pMgr->ChangeState();
	}
}

void CEnemyEyes::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// MovementBoundary와 충돌했거나, 가까운 상태에서 Chase 상태라면 Idle로 변경 or 
	// 일정 시간 반대방향으로 Chase

	if (m_EnemyData->GetIsDead() == true)
	{
		return;
	}
}

void CEnemyEyes::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_EnemyData->GetIsDead() == true)
		return;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();

		if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
		{
			// FLOWER 타입은 추적모드에서 전환 X
			if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::RANGED_ATTACK))
			{
				pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
				pMgr->ChangeState();
			}
			return;
		}

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
	}
}

void CEnemyEyes::SaveToLevelFile(FILE* _File)
{
}

void CEnemyEyes::LoadFromLevelFile(FILE* _File)
{
}
