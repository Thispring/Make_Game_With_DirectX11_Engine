#pragma once
#include "CScript.h"
class CPortal :
    public CScript
{

public:
    //=========
    // 멤버 함수
    //=========
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPortal);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CPortal();
    virtual ~CPortal();

};
