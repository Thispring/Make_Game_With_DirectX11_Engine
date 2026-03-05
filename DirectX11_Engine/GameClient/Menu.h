#pragma once
#include "EditorUI.h"

class Menu :
    public EditorUI
{

private:
    //=================
    // private 멤버 함수
    //=================
    void File();
    void Level();
    void View();
    void GameObject();
    void Asset();
    wstring GetAssetName(ASSET_TYPE _Type, const wstring& _Name);


public:
    //=============
    // 상속 멤버 함수
    //=============
    // Menu는 Tick을 상속 받아, 기능을 구현 (Tick_UI에는 기능 구현 X)
    virtual void Tick() override;
    virtual void Tick_UI() override;
    

    //============
    // 생성, 소멸자
    //============
    Menu();
    virtual ~Menu();
};
