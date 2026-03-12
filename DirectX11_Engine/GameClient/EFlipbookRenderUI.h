#pragma once
#include "EComponentUI.h"
#include "AFlipbook.h"

class EFlipbookRenderUI :
    public EComponentUI
{

private:
    vector<Ptr<AFlipbook>>      m_vecFlipbook;
    wstring                     m_AddFlipbookName;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    GET_SET(wstring, AddFlipbookName);


    //============
    // 생성, 소멸자
    //============
    EFlipbookRenderUI();
    virtual ~EFlipbookRenderUI();
};
