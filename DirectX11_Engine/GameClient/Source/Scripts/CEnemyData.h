#pragma once
#include "CScript.h"
#include "GameObject.h"
#include "contentEnum.h"

// 게임에 등장하는 모든 Enemy의 체력, 공격력 등의 정보를 이 객체에서 정의하고 관리합니다.
// Enemy의 타입별로 멤버를 다르게 설정할 수 있도록 설계합니다.
// 멤버는 GET_SET 함수로만 접근을 허용합니다.
class CEnemyData :
    public CScript
{

private:
    Ptr<GameObject>     m_TargetObject;
    ENEMY_TYPE          m_EnemyType;

    Vec3                m_OriginPos;
    Vec3                m_CurPos;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    float               m_Speed;
    float               m_JumpVelocity;

    bool                m_IsDead;
    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT 만큼 음수 이동

    bool                m_IsAttack;     

    float               m_VelocityY;        // Y축 수직 속도

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
    CLONE(CEnemyData);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(float, FullHP);
    GET_SET(float, CurHP);
    GET_SET(float, Damage);
    GET_SET(float, Speed);
    GET_SET(float, JumpVelocity);

    GET_SET(bool, IsDead);
    GET_SET(bool, IsFalling);
    GET_SET(bool, IsAttack);

    GET_SET(Vec3, OriginPos);
    GET_SET(Vec3, CurPos);

    GET_SET(Ptr<GameObject>, TargetObject);
    GET_SET(ENEMY_TYPE, EnemyType);

    GET_SET(float, VelocityY);

    //============
    // 생성, 소멸자
    //============
    CEnemyData();
    virtual ~CEnemyData();
};
