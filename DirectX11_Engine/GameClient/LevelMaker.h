#pragma once
#include "EditorUI.h"

class LevelMaker :
    public EditorUI
{

private:
    wstring         m_LevelName;

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
    GET_SET(wstring, LevelName);


    //============
    // 생성, 소멸자
    //============
    LevelMaker();
    virtual ~LevelMaker();
};
