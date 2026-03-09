#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CCamMoveScript.h"
#include "Scripts/CEnemyAnimator.h"
#include "Scripts/CEnemyBehavior.h"
#include "Scripts/CEnemySpawner.h"
#include "Scripts/CEnemyStatus.h"
#include "Scripts/CPlayerAnimator.h"
#include "Scripts/CPlayerController.h"
#include "Scripts/CPlayerStatus.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CEnemyAnimator");
	_vec.push_back(L"CEnemyBehavior");
	_vec.push_back(L"CEnemySpawner");
	_vec.push_back(L"CEnemyStatus");
	_vec.push_back(L"CPlayerAnimator");
	_vec.push_back(L"CPlayerController");
	_vec.push_back(L"CPlayerStatus");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CEnemyAnimator" == _strScriptName)
		return new CEnemyAnimator;
	if (L"CEnemyBehavior" == _strScriptName)
		return new CEnemyBehavior;
	if (L"CEnemySpawner" == _strScriptName)
		return new CEnemySpawner;
	if (L"CEnemyStatus" == _strScriptName)
		return new CEnemyStatus;
	if (L"CPlayerAnimator" == _strScriptName)
		return new CPlayerAnimator;
	if (L"CPlayerController" == _strScriptName)
		return new CPlayerController;
	if (L"CPlayerStatus" == _strScriptName)
		return new CPlayerStatus;
	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMMOVESCRIPT:
		return new CCamMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYANIMATOR:
		return new CEnemyAnimator;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYBEHAVIOR:
		return new CEnemyBehavior;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSPAWNER:
		return new CEnemySpawner;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSTATUS:
		return new CEnemyStatus;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERANIMATOR:
		return new CPlayerAnimator;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCONTROLLER:
		return new CPlayerController;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSTATUS:
		return new CPlayerStatus;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMMOVESCRIPT:
		return L"CCamMoveScript";
		break;

	case SCRIPT_TYPE::ENEMYANIMATOR:
		return L"CEnemyAnimator";
		break;

	case SCRIPT_TYPE::ENEMYBEHAVIOR:
		return L"CEnemyBehavior";
		break;

	case SCRIPT_TYPE::ENEMYSPAWNER:
		return L"CEnemySpawner";
		break;

	case SCRIPT_TYPE::ENEMYSTATUS:
		return L"CEnemyStatus";
		break;

	case SCRIPT_TYPE::PLAYERANIMATOR:
		return L"CPlayerAnimator";
		break;

	case SCRIPT_TYPE::PLAYERCONTROLLER:
		return L"CPlayerController";
		break;

	case SCRIPT_TYPE::PLAYERSTATUS:
		return L"CPlayerStatus";
		break;

	}
	return nullptr;
}