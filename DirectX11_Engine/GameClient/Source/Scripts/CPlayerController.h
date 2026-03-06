#pragma once
#include "CScript.h"

class CPlayerStatus;

class CPlayerController :
    public CScript
{

private:
    Ptr<CPlayerStatus> m_pStatus;
    
public:
    //=========
    // 멤버 함수
    //=========
    void Move();
    void Jump();
    void Shoot();
    void Attack();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerController);


    //============
    // 생성, 소멸자
    //============
    CPlayerController();
    virtual ~CPlayerController();
};
