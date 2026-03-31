#include "pch.h"
#include "CPlayerAnimator.h"

#include "CMeshRender.h"
#include "GameObject.h"


CPlayerAnimator::CPlayerAnimator()
	: CScript(SCRIPT_TYPE::PLAYERANIMATOR)
	, m_IsPlaying(false)
{
}

CPlayerAnimator::~CPlayerAnimator()
{
}


void CPlayerAnimator::Play()
{
	// 현재 상태에 접근해, 재생할 Flipbook의 Enum or 문자열 or 인덱스 번호를 가져옵니다.
	PLAYER_STATE Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
	int fps = 0;
	int repCount = 0;

	// 추가:
	// 상태에 따라 조건 분기하여, Flipbook의 FPS를 다르게 전달
	// 반복 여부도 따로 변수로 설정하여, Play 하나로 통일시키기
	switch (Idx)
	{
	case PLAYER_STATE::IDLE: fps = 10; repCount = -1;
		break;
	case PLAYER_STATE::WALK: fps = 10; repCount = -1;
		break;
	case PLAYER_STATE::JUMP: fps = 10; repCount = 1;
		break;
	case PLAYER_STATE::PUNCH: fps = 20; repCount = 1;
		break;
	case PLAYER_STATE::HIGH_KICK: fps = 15; repCount = 1;
		break;
	case PLAYER_STATE::MIDDLE_KICK: fps = 15; repCount = 1;
		break;
	case PLAYER_STATE::LOW_KICK: fps = 15; repCount = 1;
		break;
	case PLAYER_STATE::ENERGYBLAST_SHOT: fps = 15; repCount = 1;
		break;
	case PLAYER_STATE::END:
		break;
	default:
		break;
	}

	GetOwner()->FlipbookRender()->Play((int)Idx, fps, repCount);
}

void CPlayerAnimator::Begin()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerAnimator::Tick()
{	
	// 현재 상태가 PUNCH이고, Flipbook 재생이 끝났는지 확인
	if (m_StatusMgr->GetCurStatus()->GetFlipbookIndex() == PLAYER_STATE::PUNCH &&
		GetOwner()->FlipbookRender()->GetFinish() ||
		m_StatusMgr->GetCurStatus()->GetFlipbookIndex() == PLAYER_STATE::MIDDLE_KICK &&
		GetOwner()->FlipbookRender()->GetFinish() ||
		m_StatusMgr->GetCurStatus()->GetFlipbookIndex() == PLAYER_STATE::JUMP &&
		GetOwner()->FlipbookRender()->GetFinish() ||
		m_StatusMgr->GetCurStatus()->GetFlipbookIndex() == PLAYER_STATE::ENERGYBLAST_SHOT &&
		GetOwner()->FlipbookRender()->GetFinish()
		)
	{
		// 상태를 Idle로 변경
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
		m_StatusMgr->ChangeState();
		// 애니메이션 재생은 ChangeState 에서 Play를 호출하고 있음
		//Play();
		return;
	}	

}

void CPlayerAnimator::SaveToLevelFile(FILE* _File)
{
}

void CPlayerAnimator::LoadFromLevelFile(FILE* _File)
{
}
