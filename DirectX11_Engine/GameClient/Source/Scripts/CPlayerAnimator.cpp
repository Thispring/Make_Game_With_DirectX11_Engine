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
	// 변경사항이 있었다면 재생 초기화
	//if (m_StatusMgr->IsStateChange() == true)
	//	m_IsPlaying = false;

	if (GetOwner()->FlipbookRender()->GetFinish() == true)
		m_IsPlaying = false;

	// 만약 FlipBook이 재생 중이면 스킵
	if (m_IsPlaying)
	{
		return;
	}

	if (m_StatusMgr->GetCurStatus())
	{
		// 현재 상태에 접근해, 재생할 Flipbook의 Enum or 문자열 or 인덱스 번호를 가져옵니다.
		int Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
		GetOwner()->FlipbookRender()->Play(Idx, 10 , 1);
		m_IsPlaying = true;
	}
}

void CPlayerAnimator::SaveToLevelFile(FILE* _File)
{
}

void CPlayerAnimator::LoadFromLevelFile(FILE* _File)
{
}
