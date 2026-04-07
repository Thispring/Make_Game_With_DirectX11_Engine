#include "pch.h"
#include "CEndingLevelController.h"


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
}

void CEndingLevelController::SaveToLevelFile(FILE* _File)
{
}

void CEndingLevelController::LoadFromLevelFile(FILE* _File)
{
}
