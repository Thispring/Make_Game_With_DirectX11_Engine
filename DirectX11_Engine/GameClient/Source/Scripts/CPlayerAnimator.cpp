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
	//// 현재 상태를 확인하고,
	//// 상태 조건을 분기
	//
	//// 지금 상태가 Idle이 아니고, Flipbook 재생이 끝난상태라면, Idle로 변경
	//if (m_CurState != PLAYER_STATE::IDLE && 
	//	GetOwner()->FlipbookRender()->GetFinish() == true)
	//{
	//	m_Target->SetState(PLAYER_STATE::IDLE);
	//}

	//// 만약 현재상태와 이전 상태가 다르다면 상태를 갱신하고
	//// IsPlaying을 false로 전환
	//if (m_CurState != m_PrevState)
	//{
	//	m_PrevState = m_Target->GetState();
	//	m_IsPlaying = false;
	//}

	//// 이전 상태와 현재 상태가 같다면
	//if (m_CurState == m_PrevState)
	//{
	//	if (GetOwner()->FlipbookRender()->GetFinish() == true)
	//		m_IsPlaying = false;

	//	// 만약 FlipBook이 재생 중이면 스킵
	//	if (m_IsPlaying)
	//		return;

	//	// 이전 상태를 기준으로 FlipBook 재생
	//	// Flipbook은 1번만 재생하여, Flipbook을 상태에 따라 바로 변경할 수 있도록 하기
	//	switch (m_PrevState)
	//	{
	//	case PLAYER_STATE::IDLE:
	//		GetOwner()->FlipbookRender()->Play(0, 8.f, 1);
	//		m_IsPlaying = true;
	//		break;
	//	case PLAYER_STATE::L_MOVE:
	//	case PLAYER_STATE::R_MOVE:
	//	case PLAYER_STATE::U_MOVE:
	//	case PLAYER_STATE::D_MOVE:
	//		GetOwner()->FlipbookRender()->Play(1, 8.f, 1);
	//		m_IsPlaying = true;
	//		break;
	//	case PLAYER_STATE::JUMP:
	//		GetOwner()->FlipbookRender()->Play(2, 8.f, 1);
	//		m_IsPlaying = true;
	//		break;
	//	case PLAYER_STATE::PUNCH:
	//		GetOwner()->FlipbookRender()->Play(3, 16.f, 1);
	//		m_IsPlaying = true;
	//	}


	//}

}

void CPlayerAnimator::Begin()
{
	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerAnimator::Tick()
{
	if (m_StatusMgr->GetCurStatus())
	{
		// 현재 상태에 접근해, 재생할 Flipbook의 Enum or 문자열 or 인덱스 번호를 가져옵니다.
		int Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
		GetOwner()->FlipbookRender()->Play(Idx, 8 , -1);
	}
}

void CPlayerAnimator::SaveToLevelFile(FILE* _File)
{
}

void CPlayerAnimator::LoadFromLevelFile(FILE* _File)
{
}
