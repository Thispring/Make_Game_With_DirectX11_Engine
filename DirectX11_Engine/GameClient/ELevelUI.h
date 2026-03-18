#pragma once
#include "EAssetUI.h"

class ELevelUI :
    public EAssetUI
{

private:
    vector<string>      m_LayerNames;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    ELevelUI();
    virtual ~ELevelUI();
};
