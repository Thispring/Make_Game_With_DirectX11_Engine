#pragma once
#include "EComponentUI.h"

class EFlipbookRenderUI :
    public EComponentUI
{

private:
    //=================
    // private 멤버 함수
    //=================
    void SelectFlipbook(DWORD_PTR _ListUI, Ptr<CFlipbookRender> _pFlipbook);

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    EFlipbookRenderUI();
    virtual ~EFlipbookRenderUI();
};
