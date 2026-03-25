#include "pch.h"
#include "CPlayerController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CPlayerController::CPlayerController()
	: CScript(SCRIPT_TYPE::PLAYERCONTROLLER)
{
}

CPlayerController::~CPlayerController()
{
}


void CPlayerController::Move()
{
	if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK));
		// 상태 변경을 알림
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Jump()
{
	if (KEY_PRESSED(KEY::SPACE))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::JUMP));
		// 상태 변경을 알림
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Punch()
{
	if (KEY_TAP(KEY::Z))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::PUNCH));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Begin()
{
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();

	// 상태 매니저 콘텐츠 스크립트 클래스 등록
	m_StatusMgr = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerController::Tick()
{
	// 이전에 눌렸다면, Idle로 변경
	if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT) || KEY_RELEASED(KEY::SPACE) || 
		KEY_RELEASED(KEY::Z))
	{
		// 이전에 조건문에 있는 Key를 눌렀었고, 현재 상태가 IDLE이 아니면
		// IDLE 상태로 전환
		if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusVec((int)PLAYER_STATE::IDLE))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::IDLE));

			m_StatusMgr->ChangeState();

			// 전환이 성공했을때만 return
			return;
		}
	}

	Move();

	Jump();

	Punch();
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

