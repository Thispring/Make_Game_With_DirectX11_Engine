#include "pch.h"
#include "CGameOverController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"

CGameOverController::CGameOverController()
	: CScript(SCRIPT_TYPE::GAMEOVERCONTROLLER)
{
}

CGameOverController::~CGameOverController()
{
}

void CGameOverController::Begin()
{
}

void CGameOverController::Tick()
{
	// 특정 조건(KEY 입력 등)이 만족되면
	// MainMenu로 돌아가기
	if (KEY_PRESSED(KEY::H))
	{
		LevelMgr::GetInst()->ChangeMainMenu();
	}

	// 자식인 Dummy Player Flipbook 재생
	GetOwner()->GetChild(0)->FlipbookRender()->Play(0, 8, 0);
}

void CGameOverController::SaveToLevelFile(FILE* _File)
{
}

void CGameOverController::LoadFromLevelFile(FILE* _File)
{
}
