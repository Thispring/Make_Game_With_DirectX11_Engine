#include "pch.h"
#include "CPlayerController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "CEnergyBlast.h"

CPlayerController::CPlayerController()
	: CScript(SCRIPT_TYPE::PLAYERCONTROLLER)
	, m_TempDir(1)
{
}

CPlayerController::~CPlayerController()
{
}


void CPlayerController::Move()
{
	// 걷기는 누르고 있는 동안 상태 유지
	// 방향 정보는 멤버 변수로 설정하고 알맞은 값을 Setter로 전달
	// m_TempDir은 1또는 -1을 보장해야함

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_TempDir = 1;

		// 방향 정보 Setter
		m_PlayerData->SetDirNum(m_TempDir);
		if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK));
			m_StatusMgr->ChangeState();
		}
	}
	else if (KEY_PRESSED(KEY::LEFT))
	{
		m_TempDir = -1;

		// 방향 정보 Setter
		m_PlayerData->SetDirNum(m_TempDir);
		if (m_StatusMgr->GetCurStatus() != m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK))
		{
			m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::WALK));
			m_StatusMgr->ChangeState();
		}
	}
	else
	{
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

void CPlayerController::EnergyBlastShot()
{
	if (KEY_TAP(KEY::C))
	{
		// PlayerData 멤버의 Prefab을 Instantiate의 인자로 전달
		Ptr<APrefab> pBlast = m_PlayerData->GetEnergyBlast();
		// 보유하고 있는 CEnergyBlast Script에 접근
		// SetUp 함수의 인자로, 방향정보를 전달
		// 방향정보는 Player 본인의 Transform 방향 * 방향키에 따라 설정된 값을 곱함
		// ex) 왼쪽이면 GetDirNum으로 -1이 곱해져 전달


		// 자식 오브젝트인 Anchor는 index 0번째를 보장해야 합니다.
		Vec3 vAnchorPos = GetOwner()->GetChild(0)->Transform()->GetWorldPos();
		Vec3 vAnchorScale = GetOwner()->GetChild(0)->Transform()->GetWorldScale();

		Vec3 vDir = Transform()->GetDir(DIR::RIGHT);
		vDir *= m_PlayerData->GetDirNum();

		// 1. Prefab에서 복제(Instantiate)하여 새로운 오브젝트 생성
		// Prefab에서 복사한 GameObject에 접근해야하기 때문에
		// CScript에서 GameObject 포인터를 반환하는 InstantiateObject 함수 추가
		// void Instantiate함수를 사용하면, 복제가 아닌, Prefab이 가리키는 같은 GameObject에 참조

		// 자식오브젝트의 위치와 스케일을 전달해야함
		// 인덱스 요청은 InstantiateObject 함수의 매개변수로 하고 있음
		GameObject* pBlastObj = InstantiateObject(pBlast.Get(), 5, vAnchorPos + vAnchorScale * vDir);
		//GameObject* pBlastObj = InstantiateObject(pBlast.Get(), 4, vAnchorPos + vMyScale * 0.5f * vDir);
			
		// 2. 복제된 오브젝트의 스크립트에 SetUp 호출
		pBlastObj->GetScript<CEnergyBlast>()->SetUp(vDir);


		m_StatusMgr->SetCurStatus(m_StatusMgr->GetStatusVec((int)PLAYER_STATE::ENERGYBLAST_SHOT));
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

	EnergyBlastShot();
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

