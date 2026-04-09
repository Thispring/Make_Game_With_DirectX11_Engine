#include "pch.h"
#include "GameMgr.h"
#include "LevelMgr.h"
#include "AssetMgr.h"


GameMgr::GameMgr()
	: m_Player(nullptr)
	, m_vecSpawnEnemy{}
	, m_vecSavePoint{}
	, m_vecPlayerPos{}
{
}

GameMgr::~GameMgr()
{
}

void GameMgr::Init()
{
	//====================================
	// Init은 Editor 모드에서 사용,
	// LevelPlayInit은 Level Play 때 사용
	//====================================

	
	//====================================
	// 현재 Level 이 아래 Level이면 바로 리턴
	//====================================
	if (LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\MainMenu.lv"
		|| LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\Ending.lv"
		|| LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\GameOver.lv")
			return;


	m_Player = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	m_PlayerData = m_Player->GetScript<CPlayerData>();

	assert(m_Player != nullptr && "Not find Player Object");
	assert(m_PlayerData != nullptr && "Not find PlayerData");
}

void GameMgr::LevelPlayInit()
{
	// Level Play 상태이면, Level이 복사되기에 LevelPlayInit을 호출하여
	// 복사된 Player 재등록
	m_Player = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	m_PlayerData = m_Player->GetScript<CPlayerData>();

	assert(m_Player != nullptr && "Not find Player Object");
	assert(m_PlayerData != nullptr && "Not find PlayerData");
}


// Level이 변경되면서 등록이 달라짐,
// Editor, Play 구분하여 멤버 선언 or Init에서 초기화 X
void GameMgr::RegisterPlayerSave(Vec3 _SavePos)
{
	// 디버그용으로 현재 위치 저장
	m_vecPlayerPos.push_back(m_PlayerData->GetCurPos());

	// Player는 OriginPos 좌표로 부활하기 때문에
	// OriginPos를 변경
   	m_PlayerData->SetOriginPos(_SavePos);

}

void GameMgr::ClearLevelPlay()
{
	// 현재 Level이 Play -> Stop 상태로 전환되었을 때
	// Level에 남아있는 소환된 Prefab을 제거하거나, 
	// enemy 각자의 위치를 초기화 하는 로직을 실행합니다.
	// 투사체는 겹치지 않는 Layer 번호를 가지고 있기에
	// Level에 해당 Layer에 오브젝트를 모두 삭제요청

	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();

	if (pLevel->GetKey() == L"Level\\Normal_Stage_0.lv")
	{
		// assert는 조건이 false일 때만 실행(중단)됩니다.
		// L"Level\\Normal_Stage_0.lv"가 아닌곳에서 호출되면 크래시
		//assert(pLevel->GetKey() != L"Level\\Normal_Stage_0.lv");

		pLevel->DestroyAllObjectsInLayer((int)LEVEL_0_LAYER::PLAYER_PROJECTILE);
		pLevel->DestroyAllObjectsInLayer((int)LEVEL_0_LAYER::ENEMY_PROJECTILE);
	}

	// Level 종료 시 static 리스트 정리
	CEnemyData::ClearAllInstances();
}

void GameMgr::PlayerRespawnEvent()
{
	// Player가 건들 수 없는 Level 바깥 영역에 Dummy Enemy를 하나 생성하고
	// 이 객체의 주소에 접근해 CEnemyData에 있는 m_isPlayerRespawn을 접근합니다.
	//
	// 문제점: 이미 비활성화라면 GameObject Tick에서 리턴을 시키므로, 비활성 Enemy는 Tick에서
	// 신호를 받을 수 없음, 그렇다고 GameObject에 부활신호를 추가하면 객체 설계에 위반
	// 
	//==========================================================
	// Player Respawn 시, CEnemyData의 static 리스트를 통해
	// 현재 Level의 모든 Enemy를 일괄 리셋합니다.
	// 비활성화된 Enemy도 static 리스트에 남아있으므로 접근 가능
	//==========================================================
	CEnemyData::ResetAllEnemies();
}
