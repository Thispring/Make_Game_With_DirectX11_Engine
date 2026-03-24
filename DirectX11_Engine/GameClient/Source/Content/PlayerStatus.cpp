#include "pch.h"
#include "PlayerStatus.h"
#include "LevelMgr.h"

PlayerStatus::PlayerStatus()
{
	if (m_PlayerData == nullptr)
	{
		Ptr<GameObject> pObj = LevelMgr::GetInst()->FindObjectByName(L"Player");
		m_PlayerData = pObj->GetScript<CPlayerData>();
	}

}

PlayerStatus::~PlayerStatus()
{

}

void PlayerStatus::Init()
{
}
