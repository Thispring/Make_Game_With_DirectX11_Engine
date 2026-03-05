#pragma once
#include "EAssetUI.h"

class ETileMapUI :
    public EAssetUI
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    ETileMapUI();
    virtual ~ETileMapUI();
};
