#pragma once
#include "EditorUI.h"

class FlipbookMaker :
    public EditorUI
{

private:
    //==============
    // Make Flipbook
    //==============
    wstring     m_SpriteName;
    wstring     m_FlipbookName;
    int         m_SpriteCount;

    //=================
    // private 멤버 함수
    //=================
    void SettingClear();

public:
    //=========
    // 멤버 함수
    //=========


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    GET_SET(wstring, SpriteName);
    GET_SET(wstring, FlipbookName);
    GET_SET(int, SpriteCount);


    //============
    // 생성, 소멸자
    //============
    FlipbookMaker();
    virtual ~FlipbookMaker();
};
