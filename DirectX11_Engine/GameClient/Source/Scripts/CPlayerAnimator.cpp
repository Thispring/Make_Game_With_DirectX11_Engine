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


void CPlayerAnimator::CheckState()
{

}

void CPlayerAnimator::Begin()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerAnimator::Tick()
{	
	if (GetOwner()->FlipbookRender()->GetFinish() == true)
		m_IsPlaying = false;

	//// 만약 FlipBook이 재생 중이면 스킵
	//if (m_IsPlaying)
	//{
	//	return;
	//}

	if (m_StatusMgr->GetCurStatus())
	{
		// 현재 상태에 접근해, 재생할 Flipbook의 Enum or 문자열 or 인덱스 번호를 가져옵니다.
		PLAYER_STATE Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
		int fps = 0;

		// 추가:
		// 상태에 따라 조건 분기하여, Flipbook의 FPS를 다르게 전달
		switch (Idx)
		{
		case PLAYER_STATE::IDLE: fps = 10;
			break;
		case PLAYER_STATE::WALK: fps = 10;
			break;
		case PLAYER_STATE::JUMP: fps = 10;
			break;
		case PLAYER_STATE::PUNCH: fps = 20;
			break;
		case PLAYER_STATE::HIGH_KICK: fps = 20;
			break;
		case PLAYER_STATE::MIDDLE_KICK: fps = 20;
			break;
		case PLAYER_STATE::LOW_KICK: fps = 20;
			break;
		case PLAYER_STATE::END:
			break;
		default:
			break;
		}

		if (m_IsPlaying == false && m_StatusMgr->IsStateChange() == true)
			GetOwner()->FlipbookRender()->Play((int)Idx, fps, 1);

		m_IsPlaying = true;
	}

}

void CPlayerAnimator::SaveToLevelFile(FILE* _File)
{
}

void CPlayerAnimator::LoadFromLevelFile(FILE* _File)
{
}
