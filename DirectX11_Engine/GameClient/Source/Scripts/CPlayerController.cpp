#include "pch.h"
#include "CPlayerController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "CEnergyBlast.h"

CPlayerController::CPlayerController()
	: CScript(SCRIPT_TYPE::PLAYERCONTROLLER)
	, m_TempDir(1)

	// 초기화는 Z, X, C로 고정
	, m_PunchKey(KEY::Z)
	, m_KickKey(KEY::X)
	, m_BlastShotKey(KEY::C)

	, m_bHasDied(false)
	, m_bKeyRevealed{ false, false, false }
{
}

CPlayerController::~CPlayerController()	
{
}


void CPlayerController::Move()
{
	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_TempDir = 1;
		m_PlayerData->SetDirNum(m_TempDir);
        if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::WALK));
			m_StatusMgr->ChangeState();
		}
	}
	else if (KEY_PRESSED(KEY::LEFT))
	{
		m_TempDir = -1;
		m_PlayerData->SetDirNum(m_TempDir);
        if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::WALK));
			m_StatusMgr->ChangeState();
		}
	}
	else
	{
        if (m_StatusMgr->GetCurStatus() == m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
			m_StatusMgr->ChangeState();
		}
	}
}

void CPlayerController::Jump()
{
	if (KEY_TAP(KEY::SPACE))
	{
		if (m_PlayerData->GetIsFalling() == true)
			return;

		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::JUMP));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Punch(KEY _key)
{
	if (KEY_TAP(_key))
	{
		m_bKeyRevealed[PUNCH_KEY] = true;

		// 콜라이더 활성화 및 IsAttack 설정은 PlayerPunchState::Begin()에서 처리
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::PUNCH));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Kick(KEY _key)
{
	if (KEY_TAP(_key))
	{
		m_bKeyRevealed[KICK_KEY] = true;

		// 콜라이더 활성화 및 IsAttack 설정은 PlayerMiddleKickState::Begin()에서 처리
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::MIDDLE_KICK));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::EnergyBlastShot(KEY _key)
{
	if (KEY_TAP(_key))
	{
		m_bKeyRevealed[BLAST_SHOT_KEY] = true;

		Ptr<APrefab> pBlast = m_PlayerData->GetEnergyBlast();

		Vec3 vAnchorPos   = GetOwner()->GetChild(PLAYER_PROJECTILE_ANCHOR)->Transform()->GetWorldPos();
		Vec3 vAnchorScale = GetOwner()->GetChild(PLAYER_PROJECTILE_ANCHOR)->Transform()->GetWorldScale();

		Vec3 vDir = Transform()->GetDir(DIR::RIGHT);
		vDir *= m_PlayerData->GetDirNum();

		GameObject* pBlastObj = InstantiateObject(pBlast.Get(), 5, vAnchorPos + vAnchorScale * vDir);
		pBlastObj->GetScript<CEnergyBlast>()->SetUp(vDir);

		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::ENERGYBLAST_SHOT));
		m_StatusMgr->ChangeState();
	}
}

void CPlayerController::Begin()
{
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();
	m_StatusMgr  = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerController::Tick()
{
	// Animator가 ChangeState(Idle/Walk)를 호출할 때까지 모든 입력 차단
	// 잠금 대상: Jump, Punch, Kick, EnergyBlast 애니메이션 재생 중
	if (m_StatusMgr->IsInputLocked())
		return;

	Move();
	Jump();
	Punch(m_PunchKey);
	Kick(m_KickKey);
	EnergyBlastShot(m_BlastShotKey);
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

void CPlayerController::ResetKeyRevealed()
{
	for (int i = 0; i < 3; ++i)
		m_bKeyRevealed[i] = false;
}

