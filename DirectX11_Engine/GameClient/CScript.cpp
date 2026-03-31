#include "pch.h"
#include "CScript.h"
#include "TaskMgr.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "GameObject.h"

CScript::CScript(int _ScriptType)
	: Component(COMPONENT_TYPE::SCRIPT)
    , m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::Destroy()
{
    // 이미 Dead 상태라면 return
    if (GetOwner()->IsDead())
        return;
    
    // TaskMgr에게 자기 자신을 다음 프레임에 지워달라고 요청
    TaskInfo info = {};
    info.Type = TASK_TYPE::DESTROY_OBJECT;
    info.Param_0 = (DWORD_PTR)GetOwner();
    TaskMgr::GetInst()->AddTask(info);
}

void CScript::AddScriptParam(SCRIPT_PARAM _Type, void* _Data, const wstring& _Desc, bool _IsInput, float _Step, const vector<wstring>& _EnumOptions)
{
    m_vecScriptParam.push_back(tScriptParam{ _Type , _Data, _Desc, _IsInput, _Step, _EnumOptions });
}

void CScript::ClearScriptParam()
{
    m_vecScriptParam.clear();
}

void CScript::Instantiate(APrefab* _Prefab, int _LayerIdx, Vec3 _WorldPos)
{
    if (_Prefab == nullptr)
        return;

    GameObject* pObject = _Prefab->Instantiate();

    pObject->Transform()->SetRelativePos(_WorldPos);

    CreateObject(pObject, _LayerIdx);
}

GameObject* CScript::InstantiateObject(APrefab* _Prefab, int _LayerIdx, Vec3 _WorldPos)
{
    if (_Prefab == nullptr)
        return nullptr;

    GameObject* pObject = _Prefab->Instantiate();

    pObject->Transform()->SetRelativePos(_WorldPos);

    CreateObject(pObject, _LayerIdx);

    return pObject;
}
