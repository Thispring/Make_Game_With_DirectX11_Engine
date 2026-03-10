#pragma once
#include "EComponentUI.h"
#include "CScript.h"

class EScriptUI :
    public EComponentUI
{

private:
    Ptr<CScript>    m_TargetScript;
    UINT            m_ItemHeight;       // ImGui UI들의 높이 설정 값


    //=================
    // private 멤버 함수
    //=================
    void AddItemHeight();

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    void SetScript(CScript* _Script);


    //===========
    // 생성, 소멸자
    //===========
    EScriptUI();
    virtual ~EScriptUI();
};
