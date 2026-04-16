#pragma once
#include "CScript.h"

class CIntroLevelController :
    public CScript
{
private:


public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CIntroLevelController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CIntroLevelController();
    virtual ~CIntroLevelController();
};
