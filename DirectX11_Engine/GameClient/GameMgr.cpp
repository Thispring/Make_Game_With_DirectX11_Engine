#include "pch.h"
#include "GameMgr.h"
#include "LevelMgr.h"
#include "AssetMgr.h"

GameMgr::GameMgr()
	: m_Player(nullptr)
	, m_vecSpawnEnemy{}
{
}

GameMgr::~GameMgr()
{
}

void GameMgr::Init()
{
	//AssetMgr::GetInst()->FIND(ALevel, L"Level\\Normal_Stage_0.lv");
	m_Player = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	assert(m_Player != nullptr && "Not find Player Object");
}
