#pragma once
#include "EditorUI.h"
#include "TreeUI.h"
#include "Asset.h"

// 등록된 모든 Asset의 정보를 표시하는 역할
class ContentUI :
    public EditorUI
{

private:
    Ptr<TreeUI>             m_Tree;

    //=================
    // private 멤버 함수
    //=================
    void SelectAsset(DWORD_PTR _Asset);

public:
    //=========
    // 멤버 함수
    //=========
    void Renew();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    ContentUI();
    virtual ~ContentUI();

};
