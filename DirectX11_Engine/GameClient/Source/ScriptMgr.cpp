#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CCamMoveScript.h"
#include "Scripts/CEnemyScript.h"
#include "Scripts/CEnemySpawner.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CPlayerFBRController.h"
#include "Scripts/CPlayerScript.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CEnemyScript");
	_vec.push_back(L"CEnemySpawner");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerFBRController");
	_vec.push_back(L"CPlayerScript");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CEnemyScript" == _strScriptName)
		return new CEnemyScript;
	if (L"CEnemySpawner" == _strScriptName)
		return new CEnemySpawner;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerFBRController" == _strScriptName)
		return new CPlayerFBRController;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMMOVESCRIPT:
		return new CCamMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSCRIPT:
		return new CEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSPAWNER:
		return new CEnemySpawner;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERFBRCONTROLLER:
		return new CPlayerFBRController;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
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

	case SCRIPT_TYPE::ENEMYSCRIPT:
		return L"CEnemyScript";
		break;

	case SCRIPT_TYPE::ENEMYSPAWNER:
		return L"CEnemySpawner";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERFBRCONTROLLER:
		return L"CPlayerFBRController";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	}
	return nullptr;
}