#pragma once
#include "EComponentUI.h"

class ECollider2DUI :
    public EComponentUI
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    ECollider2DUI();
    virtual ~ECollider2DUI();
};
