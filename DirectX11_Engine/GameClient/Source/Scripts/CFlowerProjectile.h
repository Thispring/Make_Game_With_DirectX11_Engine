#pragma once
#include "CScript.h"

class CFlowerProjectile :
    public CScript
{

private:
    Vec3        m_Dir;
    float       m_TravelTime;
    float       m_Speed;
    float       m_Damage;
    float       m_VelocityX;

    bool        m_IsCollided;
    bool        m_IsDestroy;

public:
    //=========
    // 멤버 함수
    //=========
    void SetUp(Vec3 _Dir);
    bool DestroyProjectile();
    
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CFlowerProjectile);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(float, TravelTime);
    GET_SET(float, Speed);
    GET_SET(float, Damage);
    GET_SET(float, VelocityX);
    GET_SET(bool, IsCollided);
    GET_SET(bool, IsDestroy);


    //============
    // 생성, 소멸자
    //============
    CFlowerProjectile();
    virtual ~CFlowerProjectile();
};

