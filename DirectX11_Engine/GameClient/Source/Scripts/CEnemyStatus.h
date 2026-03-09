#pragma once
#include "CScript.h"

// Enemy의 상태를 정의합니다.
// Animator에서 상태를 읽어와 알맞은 Flipbook을 재생하는데 사용
enum class ENEMY_STATE
{
    IDLE,
    MOVE,
    ATTACK,
    END,
};

class CEnemyStatus :
    public CScript
{

private:
    ENEMY_STATE         m_State;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    float               m_Speed;

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
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyStatus);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    
    //============
    // 생성, 소멸자
    //============
    CEnemyStatus();
    virtual ~CEnemyStatus();
};
