#pragma once
#include "EditorUI.h"
#include "GameObject.h"

class PrefabMaker :
    public EditorUI
{

private:
    Ptr<GameObject>     m_Object;


    //=================
    // private 멤버 함수
    //=================
    void ClearSetting();

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    GET_SET(Ptr<GameObject>, Object);


    //============
    // 생성, 소멸자
    //============
    PrefabMaker();
    virtual ~PrefabMaker();
};
