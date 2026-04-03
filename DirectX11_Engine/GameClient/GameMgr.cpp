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

	// NOTE(26-04-03):
	// GameMgr을 엔진 초기화때 같이 초기화하기 위해
	// 현재 Level이 아닌, 등록된 에셋 중 이름을 통해 Player를 등록하게 합니다.
	//Ptr<ALevel> pLevel = FIND(ALevel, L"Level\\Normal_Stage_0.lv");		// Texture 생성 시 설정한 이름 String을 입력합니다. 

	//m_Player = pLevel->FindObjectByName(L"Player");
	//m_PlayerData = m_Player->GetScript<CPlayerData>();
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
