#include "pch.h"
#include "PlayerState.h"
#include "LevelMgr.h"

PlayerState::PlayerState()
{
	if (m_PlayerData == nullptr)
	{
		Ptr<GameObject> pObj = LevelMgr::GetInst()->FindObjectByName(L"Player");
		m_PlayerData = pObj->GetScript<CPlayerData>();
	}

}

PlayerState::~PlayerState()
{

}

void PlayerState::Init()
{
}
