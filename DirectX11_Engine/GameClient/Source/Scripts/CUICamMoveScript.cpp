#include "pch.h"
#include "CUICamMoveScript.h"
#include "LevelMgr.h"
#include "GameMgr.h"

CUICamMoveScript::CUICamMoveScript()
	: CScript(SCRIPT_TYPE::UICAMMOVESCRIPT)
{
}

CUICamMoveScript::~CUICamMoveScript()
{
}

void CUICamMoveScript::Begin()
{
	//m_Target = GameMgr::GetInst()->GetPlayer();
	//Vec3 vPos = m_Target->Transform()->GetRelativePos();
	//vPos.z = -350.f;

	//Transform()->SetRelativePos(vPos);
	//Transform()->SetRelativeRot(m_Target->Transform()->GetRelativeRot());

	m_Target = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");

	// Player 게임 오브젝트를 찾아서 등록
	//m_Target = LevelMgr::GetInst()->FindObjectByName(L"Player");
}

void CUICamMoveScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// Player의 Z축을 제외하고 가져옵니다.
	Vec3 playerPos = m_Target->Transform()->GetRelativePos();
	playerPos.z = vPos.z;

	GetOwner()->Transform()->SetRelativePos(playerPos);
}

void CUICamMoveScript::SaveToLevelFile(FILE* _File)
{
}

void CUICamMoveScript::LoadFromLevelFile(FILE* _File)
{
}

