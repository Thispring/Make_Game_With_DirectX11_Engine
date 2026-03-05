#pragma once
#include "EComponentUI.h"

class ESpriteRenderUI :
    public EComponentUI
{

private:

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    ESpriteRenderUI();
    virtual ~ESpriteRenderUI();
};
