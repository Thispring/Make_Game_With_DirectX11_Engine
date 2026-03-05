#pragma once
#include "CScript.h"

class CPlayerFBRController; 

enum class PLAYER_STATE
{
    NONE,
    IDLE,
    MOVE,
    L_MOVE, R_MOVE, U_MOVE, D_MOVE,
    ATTACK,
    HIT,
    DIE,

    END
};


class CPlayerScript :
    public CScript
{
private:
    PLAYER_STATE        m_State;            // 플레이어의 상태

    bool                m_StateChanged;     // PLAYER_STATE가 바뀌었는지 체크

    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT만큼 음수 이동

    int                 m_MissileCount;     // 미사일 개수
    bool                m_MissileReturn;    // 미사일이 되돌아 왔는지 여부, 발사 후 false면 Attack 애니메이션 중지 X

public:
    //=========
    // 멤버 함수
    //=========
    void Move();
    void Jump();
    void Shoot();
    void DrawDebug();

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
    CLONE(CPlayerScript);


    //=========
    // Get, Set
    //=========
    GET_SET(PLAYER_STATE, State);
    GET_SET(bool, StateChanged);
    GET_SET(bool, MissileReturn);
    GET_SET(int, MissileCount);

    
    //============
    // 생성, 소멸자
    //============
    CPlayerScript();
    virtual ~CPlayerScript();


    //=============
    // friend class
    //=============
    friend class CPlayerFBRController;

};
