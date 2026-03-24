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


void CPlayerController::Begin()
{
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();

	//const vector<Ptr<CScript>>& vecScripts = _Object->GetScripts();

	//m_vecStatus.push_back(new PlayerIdleStatus);
	//m_vecStatus.push_back(new PlayerMoveStatus);
	// CPlayerStatus 주소를 가져와 멤버 변수 접근
	// ex) Jump 함수 호출 시, Status의 멤버인 m_IsFalling을 변경
	//m_Status = GetOwner()->GetScript<CPlayerStatus>();

}

void CPlayerController::Tick()
{

	if (KEY_PRESSED(KEY::A))
	{
		// Test Code
		// Controller와 StateManager 모두 같은 주소(PlayerData) 참조확인
		m_PlayerData->SetCurHP(0.f);
	}
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

