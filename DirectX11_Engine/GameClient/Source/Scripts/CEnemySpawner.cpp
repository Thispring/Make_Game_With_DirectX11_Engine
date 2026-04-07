#include "pch.h"
#include "CEnemySpawner.h"

#include "GameObject.h"

#include "TimeMgr.h"
#include "AssetMgr.h"

CEnemySpawner::CEnemySpawner()
	: CScript(SCRIPT_TYPE::ENEMYSPAWNER)
	, m_Demon(nullptr)
	, m_Skull(nullptr)
	, m_Flying(nullptr)
	, m_Flower(nullptr)
	, m_Boss(nullptr)
	
	, m_vecSpawnEnemy{}

	, m_spawnCount(0)
	, m_spawnTime(0)
{
}

CEnemySpawner::~CEnemySpawner()
{
}


void CEnemySpawner::SpawnEnemy()
{
	// Prefab 멤버를 이용하여 소환

}

void CEnemySpawner::Init()
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Demon, L"DemonPrefab", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Skull, L"SkullPrefab", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Flying, L"FlyingPrefab", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Flower, L"FlowerPrefab", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Boss, L"BossPrefab", true, 0.f);
}

void CEnemySpawner::Begin()
{
	
}

void CEnemySpawner::Tick()
{

}


void CEnemySpawner::SaveToLevelFile(FILE* _File)
{
}

void CEnemySpawner::LoadFromLevelFile(FILE* _File)
{
}

