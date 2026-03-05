#pragma once
#include "EAssetUI.h"

class EMaterialUI :
    public EAssetUI
{

private:
    //=================
    // private 멤버 함수
    //=================
    void SelectShader(DWORD_PTR _ListUI);
    void ShaderParameter();


public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    EMaterialUI();
    virtual ~EMaterialUI();
};
