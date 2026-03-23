#include "pch.h"
#include "PlayerStatus.h"
#include "LevelMgr.h"

PlayerStatus::PlayerStatus()
{
	// GameObject Ptr 멤버 초기화
	m_Target = LevelMgr::GetInst()->FindObjectByName(L"Player");
}

PlayerStatus::~PlayerStatus()
{

}
