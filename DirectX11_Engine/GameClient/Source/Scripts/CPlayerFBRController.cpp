#include "pch.h"
#include "CPlayerFBRController.h"

#include "CMeshRender.h"
#include "GameObject.h"


CPlayerFBRController::CPlayerFBRController()
	: CScript(SCRIPT_TYPE::PLAYERFBRCONTROLLER)
	, m_IsPlaying(false)
{
}

CPlayerFBRController::~CPlayerFBRController()
{
}


void CPlayerFBRController::CheckState()
{
	// 현재 상태를 확인하고,
	// 상태 조건을 분기
	
	// 만약 현재상태와 이전 상태가 다르다면 상태를 갱신하고
	// IsPlaying을 false로 전환
	if (m_CurState != m_PrevState)
	{
		m_PrevState = m_Target->GetState();
		m_IsPlaying = false;
	}

	// 이전 상태와 현재 상태가 같다면
	if (m_CurState == m_PrevState)
	{
		if (GetOwner()->FlipbookRender()->GetFinish() == true)
			m_IsPlaying = false;

		// 만약 FlipBook이 재생 중이면 스킵
		if (m_IsPlaying)
			return;

		// 이전 상태를 기준으로 FlipBook 재생
		switch (m_PrevState)
		{
		case PLAYER_STATE::IDLE:
			GetOwner()->FlipbookRender()->Play(1, 10.f, 1);
			m_IsPlaying = true;
			break;
		case PLAYER_STATE::MOVE:
			GetOwner()->FlipbookRender()->Play(0, 10.f, 1);
			m_IsPlaying = true;
			break;
		case PLAYER_STATE::ATTACK:
			GetOwner()->FlipbookRender()->Play(2, 10.f, 1);
			m_IsPlaying = true;
			break;
		}


	}

}

void CPlayerFBRController::Begin()
{
	// Begin에서 m_Target을 생성된 Player 오브젝트로 설정
	m_Target = GetOwner()->GetScript<CPlayerScript>();

	// PrevState의 디폴트는 IDLE로 설정 (문제가 없다면 IDLE이 CurState로 들어오기 때문)
	m_PrevState = PLAYER_STATE::IDLE;
}

void CPlayerFBRController::Tick()
{
	// 매 프레임 마다, 상태를 갱신
	m_CurState = m_Target->GetState();

	CheckState();
}