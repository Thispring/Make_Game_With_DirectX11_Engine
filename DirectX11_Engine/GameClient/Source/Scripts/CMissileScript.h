#pragma once
#include "CScript.h"
#include "CPlayerScript.h"
#include "GameObject.h"

class CMissileScript :
    public CScript
{

private:
    Vec3    m_OriginPos;    // 생성 당시 좌표
    Vec3    m_ReturnTarget; // 부메랑 처럼, 되돌아 오기 위한 위치 정보
    Vec3    m_Offset;       // TargetPos와 얼마나 떨어져 있는지
    float   m_travelTime;   // 생성되고 얼마만큼 시간이 흘렀는지

    Ptr<GameObject> m_TargetObj;    // 추적하려는 오브젝트
    Ptr<CPlayerScript> m_PlayerObj;
    GameObject* p_TargetObj;

public:
    //=========
    // 멤버 함수
    //=========
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    void Chase();

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CMissileScript);


    //=========
    // Get, Set
    //=========
    float GetTime() { return m_travelTime; }
    void SetTime(int _Time) { m_travelTime = _Time; }
    GET_SET(Vec3, ReturnTarget);

    // TargetObj Get, Set
    Ptr<GameObject> GetTargetObj() { return m_TargetObj; }
    void SetTargetObj(Ptr<GameObject> _Target) { m_TargetObj = _Target; }


    //============
    // 생성, 소멸자
    //============
    CMissileScript();
    virtual ~CMissileScript();

};
