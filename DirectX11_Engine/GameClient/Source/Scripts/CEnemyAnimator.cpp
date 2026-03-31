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

extern int g_caseCounter = 0;
void CEnemyAnimator::Play()
{


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
	case ENEMY_STATE::DEMON_ATTACK: fps = 10; repCount = 1;
		break;
	case ENEMY_STATE::DEMON_HIT: fps = 10; repCount = 1;
		break;
	case ENEMY_STATE::DEMON_DEAD: fps = 10; repCount = 0; ++g_caseCounter;
		break;

	case ENEMY_STATE::SKULL_IDLE:
		break;
	case ENEMY_STATE::SKULL_MOVE:
		break;
	case ENEMY_STATE::SKULL_JUMP:
		break;
	case ENEMY_STATE::SKULL_ATTACK:
		break;
	case ENEMY_STATE::SKULL_HIT:
		break;
	case ENEMY_STATE::SKULL_DEAD:
		break;
	case ENEMY_STATE::FLYING_IDLE:
		break;
	case ENEMY_STATE::FLYING_MOVE:
		break;
	case ENEMY_STATE::FLYING_JUMP:
		break;
	case ENEMY_STATE::FLYING_ATTACK:
		break;
	case ENEMY_STATE::FLYING_HIT:
		break;
	case ENEMY_STATE::FLYING_DEAD:
		break;
	case ENEMY_STATE::FLOWER_IDLE:
		break;
	case ENEMY_STATE::FLOWER_MOVE:
		break;
	case ENEMY_STATE::FLOWER_JUMP:
		break;
	case ENEMY_STATE::FLOWER_ATTACK:
		break;
	case ENEMY_STATE::FLOWER_HIT:
		break;
	case ENEMY_STATE::FLOWER_DEAD:
		break;
	case ENEMY_STATE::BOSS_IDLE:
		break;
	case ENEMY_STATE::BOSS_MOVE:
		break;
	case ENEMY_STATE::BOSS_JUMP:
		break;
	case ENEMY_STATE::BOSS_ATTACK:
		break;
	case ENEMY_STATE::BOSS_HIT:
		break;
	case ENEMY_STATE::BOSS_DEAD:
		break;
	default:
		break;
	}

	GetOwner()->FlipbookRender()->Play((int)Idx, fps, repCount);
}

void CEnemyAnimator::Init()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CEnemyStateManager>();
}

void CEnemyAnimator::Begin()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	//m_StatusMgr = GetOwner()->GetScript<CEnemyStateManager>();
}

void CEnemyAnimator::Tick()
{
	//Play();
}

void CEnemyAnimator::SaveToLevelFile(FILE* _File)
{
}

void CEnemyAnimator::LoadFromLevelFile(FILE* _File)
{
}
