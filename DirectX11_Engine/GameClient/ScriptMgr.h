#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMMOVESCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
};

using namespace std;

class CScript;

class ScriptMgr
{

private:


public:
	// 게임엔진에 존재하는 모든 종류의 Script의 이름을 저장하는 함수
	static void GetScriptInfo(vector<wstring>& _vec);
	// 원하는 Script 이름을 받으면, 해당 콘텐츠 Script의 클래스를 생성하는 함수
	static CScript* GetScript(const wstring& _ScriptName);
	// Script의 이름 정보를 반환하는 함수
	static wstring GetScriptName(CScript* _Script);
};
