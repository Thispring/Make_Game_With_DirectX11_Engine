#pragma once
#include "EComponentUI.h"

class ELight2DUI :
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
    ELight2DUI();
    virtual ~ELight2DUI();
};
