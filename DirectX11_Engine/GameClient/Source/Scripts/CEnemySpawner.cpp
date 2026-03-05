#include "pch.h"
#include "CEnemySpawner.h"
#include "CEnemyScript.h"
#include "GameObject.h"
#include "AssetMgr.h"

CEnemySpawner::CEnemySpawner()
	: CScript(SCRIPT_TYPE::ENEMYSPAWNER)
	, m_spawnCount(0)
	, m_spawnTime(0)
{
}

CEnemySpawner::~CEnemySpawner()
{
}


void CEnemySpawner::SpawnEnemy()
{
	GameObject* pObject = new GameObject;

	pObject->SetName(L"Enemy");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CEnemyScript);
	pObject->AddComponent(new CCollider2D);

	Vec3 vMyPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vMyRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 vMyScale = GetOwner()->Transform()->GetRelativeScale();

	// 0 ~ 30 사이의 난수를 생성하여
	// Enemy Pos를 다르게 설정

	float ranX = rand() % 100;
	float ranY = rand() % 50;

	vMyPos.x += ranX;
	vMyPos.y += ranY;

	// Transform 정보는 Spawner의 Transform으로 설정
	pObject->Transform()->SetRelativePos(vMyPos);
	pObject->Transform()->SetRelativeScale(vMyScale * 25.f);
	pObject->Transform()->SetRelativeRot(vMyRot);

	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"BotMove_Fb"));
	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"BotIdle_Fb"));
	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"BotAttack_Fb"));

	pObject->FlipbookRender()->Play(1, 8.f, -1);

	m_spawnCount++;

	// Enemy Layer 인덱스는 5
	CreateObject(pObject, 5);

	// 스폰 시간을 0으로 초기화
	m_spawnTime = 0;
}

void CEnemySpawner::Tick()
{
	// 2초 마다 생성
	if (m_spawnTime >= 2)
	{
		//SpawnEnemy();
	}
	else
	{
		m_spawnTime += 1 * DT;
	}
}

void CEnemySpawner::Begin()
{
	srand(time(NULL));
}

