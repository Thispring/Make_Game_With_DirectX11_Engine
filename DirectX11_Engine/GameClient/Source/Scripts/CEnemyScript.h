#pragma once
#include "CScript.h"
#include "TimeMgr.h"
#include "CFlipbookRender.h"

class CEnemyScript :
    public CScript
{

private:
    float   m_HP;
    float   m_Time;

public:
    //=========
    // 멤버 함수
    //=========
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    void TakeDamage(float _Damage);
   
    

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyScript);


    //============
    // 생성, 소멸자
    //============
    CEnemyScript();
    virtual ~CEnemyScript();
};
