#include "pch.h"
#include "CPlayerAnimator.h"

#include "CMeshRender.h"
#include "GameObject.h"
#include "KeyMgr.h"


CPlayerAnimator::CPlayerAnimator()
	: CScript(SCRIPT_TYPE::PLAYERANIMATOR)
	, m_IsPlaying(false)
	, m_bPrevPunchKey(false)
	, m_bPrevKickKey(false)
	, m_bPlayStarted(false)
{
}

CPlayerAnimator::~CPlayerAnimator()
{
}


void CPlayerAnimator::Play()
{
	PLAYER_STATE Idx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
	int fps      = 0;
	int repCount = 0;

	switch (Idx)
	{
	case PLAYER_STATE::IDLE:             fps = 10; repCount = -1; break;
	case PLAYER_STATE::WALK:             fps = 10; repCount = -1; break;
	case PLAYER_STATE::JUMP:             fps = 10; repCount =  0; break;
	case PLAYER_STATE::PUNCH:            fps = 20; repCount =  0; break;
	case PLAYER_STATE::HIGH_KICK:        fps = 15; repCount =  0; break;
	case PLAYER_STATE::MIDDLE_KICK:      fps = 15; repCount =  0; break;
	case PLAYER_STATE::LOW_KICK:         fps = 15; repCount =  0; break;
	case PLAYER_STATE::ENERGYBLAST_SHOT: fps = 15; repCount =  0; break;
	case PLAYER_STATE::END:
	default:
		break;
	}

	GetOwner()->FlipbookRender()->Play(m_StatusMgr->GetFlipBookIndex(), fps, repCount);
}

void CPlayerAnimator::Begin()
{
	m_StatusMgr = GetOwner()->GetScript<CPlayerStateManager>();
}

void CPlayerAnimator::Tick()
{
	// 첫 프레임: CPlayerStateManager::Begin()에서 ChangeState()를 거치지 않으므로
	// Play()가 호출되지 않은 상태 → 여기서 초기 Idle 애니메이션 재생 보장
	if (!m_bPlayStarted)
	{
		m_bPlayStarted = true;
		Play();
		return;
	}

	PLAYER_STATE curIdx = m_StatusMgr->GetCurStatus()->GetFlipbookIndex();
	bool         bFinish = GetOwner()->FlipbookRender()->GetFinish();

	// ─── 펀치 ───────────────────────────────────────────────────────────────
	if (curIdx == PLAYER_STATE::PUNCH && bFinish)
	{
		if (m_bPrevPunchKey)
		{
			// 이전 프레임에도 Z 키를 누르고 있었으면 → 같은 공격 재시작
			// ChangeState의 same-state 스킵 문제를 피하기 위해 직접 처리
			m_StatusMgr->GetCurStatus()->FinalTick();   // 콜라이더 OFF, IsAttack 해제
			m_StatusMgr->GetCurStatus()->Begin();       // 콜라이더 ON, IsAttack 설정
			Play();                                     // 애니메이션 재시작
		}
		else
		{
			// 키를 놓았으면 → Idle 전환
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
			m_StatusMgr->ChangeState();
		}

		m_bPrevPunchKey = KEY_PRESSED(KEY::Z);
		m_bPrevKickKey  = KEY_PRESSED(KEY::X);
		return;
	}

	// ─── 킥 (Middle / High / Low) ────────────────────────────────────────────
	if ((curIdx == PLAYER_STATE::MIDDLE_KICK ||
		 curIdx == PLAYER_STATE::HIGH_KICK   ||
		 curIdx == PLAYER_STATE::LOW_KICK)   && bFinish)
	{
		if (m_bPrevKickKey)
		{
			// 이전 프레임에도 X 키를 누르고 있었으면 → 같은 킥 재시작
			m_StatusMgr->GetCurStatus()->FinalTick();
			m_StatusMgr->GetCurStatus()->Begin();
			Play();
		}
		else
		{
			// 키를 놓았으면 → Idle 전환
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
			m_StatusMgr->ChangeState();
		}

		m_bPrevPunchKey = KEY_PRESSED(KEY::Z);
		m_bPrevKickKey  = KEY_PRESSED(KEY::X);
		return;
	}

	// ─── 점프 / 에너지 블라스트: 끝나면 무조건 Idle ─────────────────────────
	if ((curIdx == PLAYER_STATE::JUMP ||
		 curIdx == PLAYER_STATE::ENERGYBLAST_SHOT) && bFinish)
	{
		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusByIndex((int)PLAYER_STATE::IDLE));
		m_StatusMgr->ChangeState();

		m_bPrevPunchKey = KEY_PRESSED(KEY::Z);
		m_bPrevKickKey  = KEY_PRESSED(KEY::X);
		return;
	}

	// 이전 프레임 키 상태 갱신 (매 프레임 마지막에 수행)
	m_bPrevPunchKey = KEY_PRESSED(KEY::Z);
	m_bPrevKickKey  = KEY_PRESSED(KEY::X);
}

void CPlayerAnimator::SaveToLevelFile(FILE* _File)
{
}

void CPlayerAnimator::LoadFromLevelFile(FILE* _File)
{
}
