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
	// StateMgr 호출

	//// 현재 상태에 맞는 ENEMY_COMMON_STATE 반환
	//ENEMY_COMMON_STATE common = m_StatusMgr->GetCurCommonState();

	//ENEMY_TYPE type = m_EnemyData->GetEnemyType();

	//// 현재 상태에 맞는 ENEMY_STATE 반환
	//ENEMY_STATE state = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();

	//GetEnemyStateToParam(type, common);

	// 현재 상태에 접근해, 재생할 Flipbook의 Enum or 문자열 or 인덱스 번호를 가져옵니다.
	ENEMY_STATE Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
	int fps = 0;
	int repCount = 0;

	// 추가:
	// 상태에 따라 조건 분기하여, Flipbook의 FPS를 다르게 전달
	// 반복 여부도 따로 변수로 설정하여, Play 하나로 통일시키기
	switch (Idx)
	{
	case ENEMY_STATE::DEMON_IDLE: fps = 10; repCount = -1;
		break;
	case ENEMY_STATE::DEMON_MOVE: fps = 10; repCount = -1;
		break;
	case ENEMY_STATE::DEMON_JUMP: fps = 10; repCount = 1;
		break;
	case ENEMY_STATE::DEMON_ATTACK: fps = 10; repCount = -1;
		break;
	case ENEMY_STATE::DEMON_HIT: fps = 10; repCount = 1;
		break;
	case ENEMY_STATE::DEMON_DEAD: fps = 10; repCount = 0;
		break;

	// NOTE(26-04-02):
	// Enum을 공통으로 사용해서 크래시 발생
	// 다른 방법으로 타입별 구분하여 애니메이션 재생

	//case ENEMY_STATE::SKULL_IDLE:
	//	break;
	//case ENEMY_STATE::SKULL_MOVE:
	//	break;
	//case ENEMY_STATE::SKULL_JUMP:
	//	break;
	//case ENEMY_STATE::SKULL_ATTACK:
	//	break;
	//case ENEMY_STATE::SKULL_HIT:
	//	break;
	//case ENEMY_STATE::SKULL_DEAD:
	//	break;
	//case ENEMY_STATE::FLYING_IDLE:
	//	break;
	//case ENEMY_STATE::FLYING_MOVE:
	//	break;
	//case ENEMY_STATE::FLYING_JUMP:
	//	break;
	//case ENEMY_STATE::FLYING_ATTACK:
	//	break;
	//case ENEMY_STATE::FLYING_HIT:
	//	break;
	//case ENEMY_STATE::FLYING_DEAD:
	//	break;
	//case ENEMY_STATE::FLOWER_IDLE:
	//	break;
	//case ENEMY_STATE::FLOWER_MOVE:
	//	break;
	//case ENEMY_STATE::FLOWER_JUMP:
	//	break;
	//case ENEMY_STATE::FLOWER_ATTACK:
	//	break;
	//case ENEMY_STATE::FLOWER_HIT:
	//	break;
	//case ENEMY_STATE::FLOWER_DEAD:
	//	break;
	//case ENEMY_STATE::BOSS_IDLE:
	//	break;
	//case ENEMY_STATE::BOSS_MOVE:
	//	break;
	//case ENEMY_STATE::BOSS_JUMP:
	//	break;
	//case ENEMY_STATE::BOSS_ATTACK:
	//	break;
	//case ENEMY_STATE::BOSS_HIT:
	//	break;
	//case ENEMY_STATE::BOSS_DEAD:
	//	break;
	default:
		break;
	}

	GetOwner()->FlipbookRender()->Play((int)Idx, fps, repCount);
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
