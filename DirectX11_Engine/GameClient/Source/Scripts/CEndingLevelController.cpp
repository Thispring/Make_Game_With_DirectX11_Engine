#include "pch.h"
#include "CEndingLevelController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"

CEndingLevelController::CEndingLevelController()
	: CScript(SCRIPT_TYPE::ENDINGLEVELCONTROLLER)
{
}

CEndingLevelController::~CEndingLevelController()
{
}

void CEndingLevelController::Begin()
{
}

void CEndingLevelController::Tick()
{
	// 특정 조건(KEY 입력 등)이 만족되면
	// MainMenu로 돌아가기
	if (KEY_PRESSED(KEY::H))
	{
		LevelMgr::GetInst()->ChangeMainMenu();
	}

	// 자식인 Dummy Player Flipbook 재생
	GetOwner()->GetChild(0)->FlipbookRender()->Play(0, 8, -1);
}

void CEndingLevelController::SaveToLevelFile(FILE* _File)
{
}

void CEndingLevelController::LoadFromLevelFile(FILE* _File)
{
}
