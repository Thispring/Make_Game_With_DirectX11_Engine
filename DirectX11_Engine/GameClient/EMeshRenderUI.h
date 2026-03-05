#pragma once
#include "EComponentUI.h"

class EMeshRenderUI :
    public EComponentUI
{

private:
    //=================
    // private 멤버 함수
    //=================
    void SelectMesh(DWORD_PTR _ListUI);
    void SelectMtrl(DWORD_PTR _ListUI);

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    EMeshRenderUI();
    virtual ~EMeshRenderUI();
};
