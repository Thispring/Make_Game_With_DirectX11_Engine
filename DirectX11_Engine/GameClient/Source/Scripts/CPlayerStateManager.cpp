#include "pch.h"
#include "CPlayerStateManager.h"

#include "LevelMgr.h"

#include "Source\Content\PlayerIdleStatus.h"
#include "Source\Content\PlayerMoveStatus.h"


CPlayerStateManager::CPlayerStateManager()
	: CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
	, m_CurStatus(nullptr)
	, m_vecStatus {}
{
}

CPlayerStateManager::~CPlayerStateManager()
{
}

void CPlayerStateManager::Begin()
{
	// Player의 공유 데이터 클래스 등록
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();

	// 상태 클래스 등록
	//m_vecStatus.push_back(make_unique<PlayerIdleStatus>());
	//m_vecStatus.push_back(make_unique<PlayerMoveStatus>());

	// 현재 상태를 Idle로 등록
	//m_CurStatus = m_vecStatus[0].get();
	m_CurStatus->Begin();

}

void CPlayerStateManager::Tick()
{
	// 필요에 따라 Tick에서 m_Status의 함수를 실행합니다.
	if (m_CurStatus)
	{
		m_CurStatus->Tick();
	}
}

void CPlayerStateManager::SaveToLevelFile(FILE* _File)
{
}

void CPlayerStateManager::LoadFromLevelFile(FILE* _File)
{
}
