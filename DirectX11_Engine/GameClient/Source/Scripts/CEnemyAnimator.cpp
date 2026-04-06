#include "pch.h"
#include "CEnemyAnimator.h"
#include "contentFunc.h"

CEnemyAnimator::CEnemyAnimator()
	: CScript(SCRIPT_TYPE::ENEMYANIMATOR)
	, m_IsPlaying(false)
{
}

CEnemyAnimator::~CEnemyAnimator()
{
}

void CEnemyAnimator::Play()
{
	// 1. 상태 매니저에서 현재 공통 상태(ENEMY_STATE)와 몬스터 타입(ENEMY_TYPE)을 가져옵니다.
	ENEMY_STATE curState = m_StatusMgr->GetCurCommonState();
	ENEMY_TYPE  enemyType = m_EnemyData->GetEnemyType();

	// 2. 재생할 실제 Flipbook 인덱스를 가져옵니다.
	int flipbookIdx = m_StatusMgr->GetFlipBookIndex();

	int fps = 10;
	int repCount = 0;

	// 3. 상태(State)에 따른 1차 분기
	switch (curState)
	{
	case ENEMY_STATE::IDLE:
	case ENEMY_STATE::PATROL:
		// 4. 몬스터 타입(Type)에 따른 2차 분기 (필요 시 세부 조절)
		if (enemyType == ENEMY_TYPE::DEMON) { fps = 10; repCount = -1; }
		else if (enemyType == ENEMY_TYPE::SKULL) { fps = 10; repCount = -1; }
		else { fps = 10; repCount = -1; }
		break;

	case ENEMY_STATE::MOVE:
	case ENEMY_STATE::CHASE:
		if (enemyType == ENEMY_TYPE::DEMON) { fps = 12; repCount = -1; }
		else if (enemyType == ENEMY_TYPE::SKULL) { fps = 15; repCount = -1; }
		else { fps = 10; repCount = -1; }
		break;

	case ENEMY_STATE::JUMP:
		// JUMP 상태가 있는 타입의 처리
		fps = 10; repCount = 1;
		break;

	case ENEMY_STATE::ATTACK:
		if (enemyType == ENEMY_TYPE::DEMON) { fps = 15; repCount = -1; }
		else if (enemyType == ENEMY_TYPE::SKULL) { fps = 12; repCount = -1; }
		else { fps = 10; repCount = -1; }
		break;

	case ENEMY_STATE::HIT:
		if (enemyType == ENEMY_TYPE::DEMON) { fps = 10; repCount = 1; }
		else if (enemyType == ENEMY_TYPE::SKULL) { fps = 10; repCount = 1; }
		else { fps = 10; repCount = 1; }
		break;

	case ENEMY_STATE::DEAD:
		if (enemyType == ENEMY_TYPE::DEMON) { fps = 10; repCount = 0; }
		else if (enemyType == ENEMY_TYPE::SKULL) { fps = 10; repCount = 0; }
		else { fps = 10; repCount = 0; }
		break;
        
	case ENEMY_STATE::GHOST_SKULL:
		fps = 10; repCount = 0;
		break;

	case ENEMY_STATE::GHOST_SKULL_MOVE:
		fps = 10; repCount = -1;
		break;

	default:
		fps = 10; repCount = -1;
		break;
	}

	// 5. 최종 결정된 Flipbook Index, fps, 반복 횟수를 통해 렌더러 Play 처리
	// FlipbookRender::Play 의 경우 플립북 인덱스가 enum이라면 int 형변환이 필요할 수 있습니다.
	GetOwner()->FlipbookRender()->Play(flipbookIdx, fps, repCount);
}

void CEnemyAnimator::Init()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CEnemyStateManager>();
	m_EnemyData = GetOwner()->GetScript<CEnemyData>();
}

void CEnemyAnimator::Begin()
{
}

void CEnemyAnimator::Tick()
{

}

void CEnemyAnimator::SaveToLevelFile(FILE* _File)
{
}

void CEnemyAnimator::LoadFromLevelFile(FILE* _File)
{
}
