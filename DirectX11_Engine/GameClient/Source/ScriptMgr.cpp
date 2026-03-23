#include "pch.h"
#include "ScriptMgr.h"
#include <cstring>

#include "Scripts/CCamMoveScript.h"
#include "Scripts/CEnemyAnimator.h"
#include "Scripts/CEnemyBehavior.h"
#include "Scripts/CEnemySpawner.h"
#include "Scripts/CEnemyStatus.h"
#include "Scripts/CPlayerAnimator.h"
#include "Scripts/CPlayerController.h"
#include "Scripts/CPlayerStateManager.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CEnemyAnimator");
	_vec.push_back(L"CEnemyBehavior");
	_vec.push_back(L"CEnemySpawner");
	_vec.push_back(L"CEnemyStatus");
	_vec.push_back(L"CPlayerAnimator");
	_vec.push_back(L"CPlayerController");
	_vec.push_back(L"CPlayerStateManager");
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
	if (L"CPlayerStateManager" == _strScriptName)
		return new CPlayerStateManager;
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
	case (UINT)SCRIPT_TYPE::PLAYERSTATEMANAGER:
		return new CPlayerStateManager;
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

	case SCRIPT_TYPE::PLAYERSTATEMANAGER:
		return L"CPlayerStateManager";
		break;

	}
	return nullptr;
}

const char* ScriptMgr::GetScriptName(SCRIPT_TYPE _Type)
{
	switch (_Type)
	{
	case CAMMOVESCRIPT: return "CCamMoveScript";
		break;

	case ENEMYANIMATOR: return "CEnemyAnimator";
		break;

	case ENEMYBEHAVIOR: return "CEnemyBehavior";
		break;

	case ENEMYSPAWNER: return "CEnemySpawner";
		break;

	case ENEMYSTATUS: return "CEnemyStatus";
		break;

	case PLAYERANIMATOR: return "CPlayerAnimator";
		break;

	case PLAYERCONTROLLER: return "CPlayerController";
		break;

	case PLAYERSTATEMANAGER: return "CPlayerStateManager";
		break;

	case SCRIPT_TYPE_END: return "End";
		break;

	default:
		break;
	}

	return nullptr;
}


CScript * ScriptMgr::GetScript(const char* _strScriptName)
{
	if (0 == strcmp(_strScriptName, "CCamMoveScript"))
		return new CCamMoveScript;
	if (0 == strcmp(_strScriptName, "CEnemyAnimator"))
		return new CEnemyAnimator;
	if (0 == strcmp(_strScriptName, "CEnemyBehavior"))
		return new CEnemyBehavior;
	if (0 == strcmp(_strScriptName, "CEnemySpawner"))
		return new CEnemySpawner;
	if (0 == strcmp(_strScriptName, "CEnemyStatus"))
		return new CEnemyStatus;
	if (0 == strcmp(_strScriptName, "CPlayerAnimator"))
		return new CPlayerAnimator;
	if (0 == strcmp(_strScriptName, "CPlayerController"))
		return new CPlayerController;
	if (0 == strcmp(_strScriptName, "CPlayerStateManager"))
		return new CPlayerStateManager;
	return nullptr;
}


SCRIPT_TYPE ScriptMgr::GetScriptType(const std::wstring& _strScriptName)
{
	if (L"CCamMoveScript" == _strScriptName)
		return CAMMOVESCRIPT;
	if (L"CEnemyAnimator" == _strScriptName)
		return ENEMYANIMATOR;
	if (L"CEnemyBehavior" == _strScriptName)
		return ENEMYBEHAVIOR;
	if (L"CEnemySpawner" == _strScriptName)
		return ENEMYSPAWNER;
	if (L"CEnemyStatus" == _strScriptName)
		return ENEMYSTATUS;
	if (L"CPlayerAnimator" == _strScriptName)
		return PLAYERANIMATOR;
	if (L"CPlayerController" == _strScriptName)
		return PLAYERCONTROLLER;
	if (L"CPlayerStateManager" == _strScriptName)
		return PLAYERSTATEMANAGER;
	return SCRIPT_TYPE_END;
}


SCRIPT_TYPE ScriptMgr::GetScriptType(const char* _strScriptName)
{
	if (0 == strcmp(_strScriptName, "CCamMoveScript"))
		return CAMMOVESCRIPT;
	if (0 == strcmp(_strScriptName, "CEnemyAnimator"))
		return ENEMYANIMATOR;
	if (0 == strcmp(_strScriptName, "CEnemyBehavior"))
		return ENEMYBEHAVIOR;
	if (0 == strcmp(_strScriptName, "CEnemySpawner"))
		return ENEMYSPAWNER;
	if (0 == strcmp(_strScriptName, "CEnemyStatus"))
		return ENEMYSTATUS;
	if (0 == strcmp(_strScriptName, "CPlayerAnimator"))
		return PLAYERANIMATOR;
	if (0 == strcmp(_strScriptName, "CPlayerController"))
		return PLAYERCONTROLLER;
	if (0 == strcmp(_strScriptName, "CPlayerStateManager"))
		return PLAYERSTATEMANAGER;
	return SCRIPT_TYPE_END;
}


void ScriptMgr::GenerateScriptFiles(const std::wstring& _strScriptName)
{
	if (_strScriptName.empty()) return;
	std::wstring scriptFolder = L"C:\\Users\\141245124\\Documents\\GitHub\\Make_Game_With_DirectX11_Engine\\DirectX11_Engine\\GameClient\\Source\\Scripts\\";
	if (scriptFolder.empty()) return;

	std::wstring headerPath = scriptFolder + _strScriptName + L".h";
	std::wstring cppPath    = scriptFolder + _strScriptName + L".cpp";

	FILE* pH = nullptr;
	if (_wfopen_s(&pH, headerPath.c_str(), L"w") == 0 && pH != nullptr)
	{
		fclose(pH);
	}
	FILE* pC = nullptr;
	if (_wfopen_s(&pC, cppPath.c_str(), L"w") == 0 && pC != nullptr)
	{
		fclose(pC);
	}
}
