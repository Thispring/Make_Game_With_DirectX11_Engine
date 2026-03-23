#include "pch.h"
#include "CPlayerStateManager.h"


CPlayerStateManager::CPlayerStateManager()
	: CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
{
}

CPlayerStateManager::~CPlayerStateManager()
{
}

void CPlayerStateManager::Begin()
{
}

void CPlayerStateManager::Tick()
{
	// 필요에 따라 Tick에서 m_Status의 함수를 실행합니다.
}

void CPlayerStateManager::SaveToLevelFile(FILE* _File)
{
}

void CPlayerStateManager::LoadFromLevelFile(FILE* _File)
{
}
