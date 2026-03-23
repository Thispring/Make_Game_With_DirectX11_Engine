#include "pch.h"
#include "CPlayerController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "Source\Content\PlayerIdleStatus.h"
#include "Source\Content\PlayerMoveStatus.h"


CPlayerController::CPlayerController()
	: CScript(SCRIPT_TYPE::PLAYERCONTROLLER)
	//, m_Status(nullptr)
{
}

CPlayerController::~CPlayerController()
{
}


void CPlayerController::Begin()
{
	m_vecStatus.push_back(new PlayerIdleStatus);
	m_vecStatus.push_back(new PlayerMoveStatus);
	// CPlayerStatus 주소를 가져와 멤버 변수 접근
	// ex) Jump 함수 호출 시, Status의 멤버인 m_IsFalling을 변경
	//m_Status = GetOwner()->GetScript<CPlayerStatus>();

}

void CPlayerController::Tick()
{
	if (KEY_PRESSED(KEY::A))
	{

	}
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

