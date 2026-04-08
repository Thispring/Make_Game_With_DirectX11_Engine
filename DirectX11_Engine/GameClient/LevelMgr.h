#pragma once
#include "ALevel.h"

// Level 개념에 대해 알고 있기
class LevelMgr
	: public Singleton<LevelMgr>
{
	SINGLE(LevelMgr)
private:
	Ptr<ALevel>		m_CurLevel;
	Ptr<ALevel>		m_ShardLevel;	// AssetMgr를 통해 관리되는 Level 에셋

	LEVEL_STATE		m_LevelState;


	//=================
	// private 멤버 함수
	//=================
	void ChangeLevel(Ptr<ALevel> _NextLevel);
	void ChangeLevelState(LEVEL_STATE _NextState);

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Progress();
	Ptr<GameObject> FindObjectByName(const wstring& _Name);
	void AddNewObject(Ptr<GameObject> _Object, Ptr<ALevel> _Level, int _Layer);

	void ChangeMainMenu();
	void GameStart();
	void ChangeEnding();
	void ChangeGameOver();


	//=========
	// Get, Set
	//=========
	Ptr<ALevel> GetCurLevel() { return m_CurLevel; }
	LEVEL_STATE GetLevelState() { return m_LevelState; }


	//=============
	// friend class
	//=============
	friend class TaskMgr;
};
