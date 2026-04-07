#pragma once
#include "CScript.h"

class CEndingLevelController :
    public CScript
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEndingLevelController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CEndingLevelController();
    virtual ~CEndingLevelController();
};
