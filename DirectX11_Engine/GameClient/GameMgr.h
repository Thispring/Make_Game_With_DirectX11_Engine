#pragma once
#include "GameObject.h"
#include "Source\Scripts\CPlayerData.h"

// 게임 콘텐츠 관련 데이터를 관리합니다.
// 싱글톤으로 디자인하여 사용합니다.
class GameMgr
	: public Singleton<GameMgr>
{
	SINGLE(GameMgr)
private:
	Ptr<GameObject>				m_Player;
	Ptr<CPlayerData>			m_PlayerData;
	vector<Ptr<GameObject>>		m_vecSpawnEnemy;
	vector<Ptr<GameObject>>		m_vecSavePoint;

	// 디버그용 Player Pos 저장
	vector<Vec3>				m_vecPlayerPos;

public:
	//=========
	// 멤버 함수
	//=========
	// Level 시작 시, 필요한 멤버 데이터를 초기화
	void Init();
	void LevelPlayInit();
	void RegisterPlayerSave(Vec3 _SavePos);


	//=========
	// Get, Set
	//=========
	Ptr<GameObject> GetPlayer() { return m_Player; }
};
