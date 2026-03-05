#pragma once
#include "EAssetUI.h"

class EGraphicShaderUI
    : public EAssetUI
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    EGraphicShaderUI();
    virtual ~EGraphicShaderUI();
};
