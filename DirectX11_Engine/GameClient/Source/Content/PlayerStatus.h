#pragma once
#include "GameObject.h"
// NOTE(26-03-23): PlayerStatus를 FSM 설계방식으로 변경합니다.
// Player의 상세한 상태를 이 클래스에서 파생한 자식 클래스로 구현하고, 
// 상태 변경을 확인하여 해당 자식의 함수 ex)Enter, Tick 을 호출합니다.
// 자식의 호출함수에서 구체적인 동작을 정의합니다.
//
// PlayerStatus는 추상 클래스 입니다.
class PlayerStatus 
{

private:
    Ptr<GameObject>     m_Target;
    PlayerStatus*       m_CurState;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    float               m_Speed;

    int                 m_DeathCount;

    bool                m_IsDead;
    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT 만큼 음수 이동


public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;

    virtual void Begin() = 0;
    virtual void Tick() = 0;
    virtual void FinalTick() = 0;
    //virtual const char* GetFlipbookName() const = 0;


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
    Ptr<GameObject> GetTargetObject() { return m_Target; }


    //============
    // 생성, 소멸자
    //============
    PlayerStatus();
    virtual ~PlayerStatus();

};
