#pragma once
#include "CScript.h"

class CLightObjectMove :
    public CScript
{

public:
    //=========
    // 멤버 함수
    //=========
    void RotateTowardsMouse();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CLightObjectMove);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========



    //============
    // 생성, 소멸자
    //============
    CLightObjectMove();
    virtual ~CLightObjectMove();
};

