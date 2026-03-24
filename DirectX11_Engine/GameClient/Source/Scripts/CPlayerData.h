#pragma once
#include "CScript.h"
#include "GameObject.h"

// Script를 상속받는 콘텐츠 스크립트 Component 입니다.
// Player의 체력, 공격력 등의 정보를 이 객체에서 정의하고 관리합니다.
// 멤버는 GET_SET 함수로만 접근을 허용합니다.
class CPlayerData :
    public CScript
{

private:
    Ptr<GameObject>     m_TargetObject;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    float               m_Speed;

    int                 m_DeathCount;

    bool                m_IsDead;
    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT 만큼 음수 이동

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
    virtual void Init() override;       // Component 클래스 추상 멤버
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerData);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(float, FullHP);
    GET_SET(float, CurHP);
    GET_SET(float, Damage);
    GET_SET(float, Speed);
    GET_SET(int, DeathCount);
    GET_SET(bool, IsDead);
    GET_SET(bool, IsFalling);
    GET_SET(Ptr<GameObject>, TargetObject);


    //============
    // 생성, 소멸자
    //============
    CPlayerData();
    virtual ~CPlayerData();
};
