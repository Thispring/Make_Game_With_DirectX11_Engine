#include "pch.h"
#include "GameObject.h"
#include "Device.h"

#include "LevelMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"
#include "CollisionMgr.h"

#include "Source/Scripts/CCamMoveScript.h"
#include "Source/Scripts/CPlayerStatus.h"
#include "Source/Scripts/CEnemyScript.h"
#include "Source/Scripts/CEnemySpawner.h"
#include "Source/Scripts/CPlayerFBRController.h"
#include "CSpriteRender.h"

LevelMgr::LevelMgr()
	: m_LevelState(LEVEL_STATE::STOP)
{
}

LevelMgr::~LevelMgr()
{
}

// 모든 Layer를 검색해서
// 매개변수로 받은 이름의 오브젝트 래퍼런스를 반환
Ptr<GameObject> LevelMgr::FindObjectByName(const wstring& _Name)
{
	return m_CurLevel->FindObjectByName(_Name);
}

void LevelMgr::ChangeLevelState(LEVEL_STATE _NextState)
{
	// 이미 같은 상태였다면 함수 실행 X
	if (m_LevelState == _NextState)
		return;

	// Stop -> Play 전환
	if (m_LevelState == LEVEL_STATE::STOP && _NextState == LEVEL_STATE::PLAY)
	{
		// 원본 Level의 복제본을 만들고 현재 Level로 가리키게 한다.
		m_CurLevel = m_ShardLevel->Clone();
		m_CurLevel->SetChanged();
		m_CurLevel->Begin();
	}
	// Play or Pause -> Stop 전환
	else if ((m_LevelState == LEVEL_STATE::PLAY || m_LevelState == LEVEL_STATE::PAUSE)
		&& _NextState == LEVEL_STATE::STOP)
	{
		m_CurLevel = m_ShardLevel;
		m_CurLevel->SetChanged();
	}

	m_LevelState = _NextState;
}

void LevelMgr::ChangeLevel(Ptr<ALevel> _NextLevel)
{
	m_CurLevel = m_ShardLevel = _NextLevel;

	m_LevelState = LEVEL_STATE::STOP;
}

void LevelMgr::Init()
{
}

void LevelMgr::Progress()
{
	if (m_CurLevel == nullptr)
		return;

	// 이전에 등록된 모든 오브젝트들 vector에서 제거
	m_CurLevel->Deregister();

	// 레벨 안에 있는 오브젝트들이 이번 DT 동안 할 일 수행
	// (26-03-03): 레벨의 상태가 Play일 때만 Level의 Tick을 수행합니다.
	if (m_LevelState == LEVEL_STATE::PLAY)
		m_CurLevel->Tick();
	
	// Object의 Component가 호출하는 것이므로(Render 관련 연산을 FinalTick에서 진행), 
	// FinalTick은 계속 호출합니다.
	m_CurLevel->FinalTick();

	// 게임 오브젝트들의 충돌 판정
	if (m_LevelState == LEVEL_STATE::PLAY)
		CollisionMgr::GetInst()->Progress(m_CurLevel);
}
