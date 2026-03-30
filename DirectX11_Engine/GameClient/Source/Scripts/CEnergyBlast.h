#pragma once
#include "CScript.h"

// Player의 발사체 스크립트
// 발사체에 필요한 멤버:
// 발사되고 지난 시간, 속력, 충돌 여부, 파괴되었는지 여부(충돌해서 파괴 or 시간 지나서 파괴)
// 데미지, 중력 가속도, X축 수직 속도

class CEnergyBlast :
    public CScript
{

private:
    Vec3 m_Dir;
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

    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnergyBlast);

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
    CEnergyBlast();
    virtual ~CEnergyBlast();
};

