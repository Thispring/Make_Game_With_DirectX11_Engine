#pragma once
#include "CScript.h"

class CEnemyStateManager;

class CEnemyBehavior :
    public CScript
{

private:
    Ptr<CEnemyStateManager> m_Status;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyBehavior);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    

    //============
    // 생성, 소멸자
    //============
    CEnemyBehavior();
    virtual ~CEnemyBehavior();
};
