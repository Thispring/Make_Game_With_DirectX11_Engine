#include "pch.h"
#include "CEnemyEyes.h"

#include "GameObject.h"

#include "CEnemyStateManager.h"
#include "TimeMgr.h"


CEnemyEyes::CEnemyEyes()
	: CScript(SCRIPT_TYPE::ENEMYEYES)
{
	m_fTimeSinceLastDetect = 1000.f;
	m_fDetectGraceTime = 0.18f; // 허용 시간
	m_bPlayerRecentlyDetected = false;
}

CEnemyEyes::~CEnemyEyes()
{
}

void CEnemyEyes::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_EnemyData->GetIsDead() == true)
		return;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// 감지: 타이머 리셋
		m_bPlayerRecentlyDetected = true;
		m_fTimeSinceLastDetect = 0.f;

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
	// 플레이어와 지속적으로 겹칠 경우 타이머 리셋
	if (_OtherCollider && _OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		m_bPlayerRecentlyDetected = true;
		m_fTimeSinceLastDetect = 0.f;
	}

	if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLYING)
	{
		int a = 0;
	}

	if (m_EnemyData->GetIsDead() == true)
	{
		return;
	}
}

void CEnemyEyes::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// 디버그 출력: 누가 EndOverlap 호출했는지 확인
	std::ostringstream oss;
	oss << "CEnemyEyes::EndOverlap called. EnemyType=" << (int)m_EnemyData->GetEnemyType()
		<< " CurState=" << (int)GetOwner()->GetParent()->GetScript<CEnemyStateManager>()->GetCurCommonState()
		<< " OtherLayer=" << _OtherCollider->GetOwner()->GetLayerIdx() << "\n";
	OutputDebugStringA(oss.str().c_str());

	if (m_EnemyData->GetIsDead() == true)
		return;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// EndOverlap도 호출되면 감지 플래그 해제 및 타이머 시작
		m_bPlayerRecentlyDetected = false;
		m_fTimeSinceLastDetect = 0.f;

		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();
		ENEMY_STATE curState = pMgr->GetCurCommonState();

		// 보호: HIT / DEAD / GHOST 상태에서는 강제 전환하지 않음
		if (curState == ENEMY_STATE::HIT
			|| curState == ENEMY_STATE::DEAD
			|| curState == ENEMY_STATE::GHOST_SKULL
			|| curState == ENEMY_STATE::GHOST_SKULL_MOVE)
			return;

		// FLOWER 타입은 기존 로직 유지 (RANGED_ATTACK -> IDLE)
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

		// FLYING 특화 처리:
		// FLYING은 플레이어를 감지하면 ATTACK 상태로 들어가는 경우가 많아서,
		// PLAYER EndOverlap 시 ATTACK 또는 CHASE 상태이면 즉시 IDLE로 전환합니다.
		if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLYING)
		{
			if (curState == ENEMY_STATE::ATTACK || curState == ENEMY_STATE::CHASE)
			{
				// 중복 전환 방지
				if (pMgr->GetCurStatus() != pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE))
				{
					pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
					pMgr->ChangeState();
					// Tick에서 중복 전환되지 않게 타이머 초기화
					m_fTimeSinceLastDetect = 1000.f;
				}
			}
			return;
		}

		// 기존: 현재 CHASE 중일 때만 IDLE로 전환 (다른 상태에서 호출 방지)
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

	// 초기값
	m_fTimeSinceLastDetect = 1000.f;
	m_fDetectGraceTime = 0.18f;
	m_bPlayerRecentlyDetected = false;

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

	// 타이머 업데이트
	if (!m_bPlayerRecentlyDetected)
	{
		m_fTimeSinceLastDetect += DT;
	}

	// 일정 시간 동안 플레이어 감지가 없으면 EndOverlap과 동일하게 상태 전환 수행
	if (m_fTimeSinceLastDetect > m_fDetectGraceTime)
	{
		Ptr<CEnemyStateManager> pMgr = GetOwner()->GetParent()->GetScript<CEnemyStateManager>();
		ENEMY_STATE curState = pMgr->GetCurCommonState();

		if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::FLOWER)
		{
			if (curState == ENEMY_STATE::RANGED_ATTACK)
			{
				pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
				pMgr->ChangeState();
			}
		}
		else
		{
			if (curState == ENEMY_STATE::CHASE)
			{
				pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
				pMgr->ChangeState();
				// 중복 전환 방지
				m_fTimeSinceLastDetect = 1000.f;
			}
		}

		m_bPlayerRecentlyDetected = false;
	}
}

void CEnemyEyes::SaveToLevelFile(FILE* _File)
{
}

void CEnemyEyes::LoadFromLevelFile(FILE* _File)
{
}