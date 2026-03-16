#pragma once
#include "EditorUI.h"
#include "GameObject.h"
#include "EComponentUI.h"
#include "EScriptUI.h"

class GameObjectMaker :
    public EditorUI
{

private:
	Ptr<GameObject>				m_pObject;
	COMPONENT_TYPE				m_ComType;

	Ptr<EComponentUI>			m_arrComUI[(UINT)COMPONENT_TYPE::END];
	vector<Ptr<EScriptUI>>		m_vecScriptUI;
	
	wstring						m_LevelName;
	wstring						m_ObjectName;

	int							m_LayerIdx;

	bool						m_IsComAdd;


	//=======
	// Script
	//=======
	//vector<string> m_vecScriptName;
	vector<const char*> m_vecScriptName;	// List로 활용할 이름 vector
	const char* m_ContnentScriptName;
	wstring m_AddScriptName;

	bool m_IsContentScriptAdd;
	bool m_IsScriptAdd;

	SCRIPT_TYPE m_ScriptType;
	
	bool m_IsObjectMake;

	//=================
	// private 멤버 함수
	//=================
	void SettingClear();

public:
	//=========
	// 멤버 함수
	//=========
	void SetTargetObject(Ptr<GameObject> _Object);
	void CreateChildUI();


	//=============
	// 상속 멤버 함수
	//=============
	virtual void Tick_UI() override;


	//=========
	// Get, Set
	//=========
	GET_SET(int, LayerIdx);
	GET_SET(wstring, LevelName);
	GET_SET(wstring, ObjectName);
	GET_SET(COMPONENT_TYPE, ComType);
	bool IsComAdd() 
	{ 
		bool _ComAdd = m_IsComAdd;
		m_IsComAdd = false;
		return _ComAdd; 
	};
	void SetComAdd() { m_IsComAdd = true; }


	GET_SET(const char*, ContnentScriptName);
	GET_SET(wstring, AddScriptName);
	GET_SET(SCRIPT_TYPE, ScriptType);
	bool IsContentScriptAdd()
	{ 
		bool _AddConScript = m_IsContentScriptAdd;
		m_IsContentScriptAdd = false;
		return _AddConScript;
	};
	void SetContentScriptAdd() { m_IsContentScriptAdd = true; }

	bool IsScriptAdd()
	{ 
		bool _AddScript = m_IsScriptAdd;
		m_IsScriptAdd = false;
		return _AddScript;
	};
	void SetScriptAdd() { m_IsScriptAdd = true; }

	bool IsObjectMake()
	{ 
		bool _AddObject = m_IsObjectMake;
		m_IsObjectMake = false;
		return _AddObject;
	};
	void SetObjectMake() { m_IsObjectMake = true; }


	//============
	// 생성, 소멸자
	//============
	GameObjectMaker();
	virtual ~GameObjectMaker();
};
