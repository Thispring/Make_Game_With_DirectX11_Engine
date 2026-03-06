#pragma once
#include "CScript.h"

class CPlayerFBRController;

// FlipbookRenderController에게 전달할 Player의 상태 정보
enum class PLAYER_STATE
{
    NONE,
    IDLE,
    // 콤보 기술을 사용하기 위해 MOVE 상태를 4개로 나누어 사용
    L_MOVE, R_MOVE, U_MOVE, D_MOVE,
    JUMP,
    ATTACK,
    // 콤보 기술명으로 공격관련 enum 정의
    PUNCH, KICK, 
    HIT,
    DIE,

    END
};

class CPlayerStatus :
    public CScript
{

private:
    PLAYER_STATE        m_State;

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
    /*************************************************
    * override 키워드는 이 클래스의 함수가
    * 오버라이드 되었다는 것을 시각적으로 표시 (가독성 증가)
    *************************************************/
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerStatus);


    //=========
    // Get, Set
    //=========
    GET_SET(PLAYER_STATE, State);
    GET_SET(float, FullHP);
    GET_SET(float, CurHP);
    GET_SET(float, Damage);
    GET_SET(float, Speed);
    GET_SET(int, DeathCount);
    GET_SET(bool, IsDead);
    GET_SET(bool, IsFalling);


    //============
    // 생성, 소멸자
    //============
    CPlayerStatus();
    virtual ~CPlayerStatus();


    //=============
    // friend class
    //=============
    friend class CPlayerFBRController;

};
