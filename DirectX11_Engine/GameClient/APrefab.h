#pragma once

#include "Asset.h"
#include "GameObject.h"

class APrefab :
    public Asset
{

private:
    Ptr<GameObject>	    m_ProtoObj;

public:
    //=========
    // 멤버 함수
    //=========
    GameObject* Instantiate();


    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;
    CLONE(APrefab);


    //=========
    // Get, Set
    //=========
    void SetObject(Ptr<GameObject> _Object) { m_ProtoObj = _Object; }
    Ptr<GameObject> GetGameObject() { return m_ProtoObj; }


    //============
    // 생성, 소멸자
    //============
    APrefab();
    virtual ~APrefab();
};
