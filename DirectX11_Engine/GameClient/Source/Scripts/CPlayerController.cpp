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
	// 걷기는 누르고 있는 동안 상태 유지
	if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
	{
		if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK));
			m_StatusMgr->ChangeState();
		}
	}
	else
	{
		// LEFT/RIGHT 가 눌려있지 않다면, 걷기 상태에서만 IDLE로 전환
		if (m_StatusMgr->GetCurStatus() == m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::IDLE));
			m_StatusMgr->ChangeState();
		}
	}
}

void CPlayerController::Jump()
{
	// 점프는 TAP으로 트리거 (원샷)
	if (KEY_TAP(KEY::SPACE))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::JUMP));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Punch()
{
	// 펀치도 TAP으로 트리거 (원샷)
	if (KEY_TAP(KEY::Z))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::PUNCH));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Kick()
{
	// Key 조합에 따라 다른 Kick 동작 나타나게 구현
	if (KEY_TAP(KEY::X))
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::MIDDLE_KICK));
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
	// 단일 프레임 키 릴리즈에 의존하여 상태를 되돌리는 로직 제거.
	// 대신 지속 입력은 Move()에서, 원샷 입력은 Tap에서 처리.
	// 상태에서의 복귀는 상태 클래스(예: PlayerPunchState::FinalTick 또는 애니메이션 완료)에서 관리하는 것이 권장됩니다.

	// 연속 입력 처리 (걷기)
	Move();

	// 원샷 입력 처리 (점프, 펀치)
	Jump();
	Punch();
	Kick();
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

