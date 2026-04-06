#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMMOVESCRIPT,
	ENEMYANIMATOR,
	ENEMYBEHAVIOR,
	ENEMYDATA,
	ENEMYEYES,
	ENEMYSPAWNER,
	ENEMYSTATEMANAGER,
	ENERGYBLAST,
	FLOWERPROJECTILE,
	LIGHTOBJECTMOVE,
	MAINMENUCONTROLLER,
	PARALLAXLAYER,
	PLAYERANIMATOR,
	PLAYERCONTROLLER,
	PLAYERDATA,
	PLAYERMELEETRIGGER,
	PLAYERSTATEMANAGER,
	SAVEPOINT,
	UICAMMOVESCRIPT,
	UIOVERLAYCONTROLLER,

	SCRIPT_TYPE_END
};

using namespace std;

class CScript;

class ScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
	static const char* GetScriptName(SCRIPT_TYPE _Type);
	static CScript * GetScript(const char* _strScriptName);
	static SCRIPT_TYPE GetScriptType(const char* _strScriptName);
	static SCRIPT_TYPE GetScriptType(const wstring& _strScriptName);
	static void GenerateScriptFiles(const std::wstring& _strScriptName);

};
