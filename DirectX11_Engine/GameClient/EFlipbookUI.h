#pragma once
#include "EAssetUI.h"

class EFlipbookUI :
    public EAssetUI
{

private:
    int     m_CurFrame;
    float   m_Elapsed;
    float   m_FrameInterval;
    wstring m_PrevFlipbookKey;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    GET_SET(float, FrameInterval);
    

    //============
    // 생성, 소멸자
    //============
    EFlipbookUI();
    virtual ~EFlipbookUI();
};
