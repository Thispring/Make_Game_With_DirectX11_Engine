#pragma once
#include "Source\Scripts\CPlayerData.h"
// NOTE(26-03-23): PlayerState를 FSM 설계방식으로 변경합니다.
// Player의 상세한 상태를 이 클래스에서 파생한 자식 클래스로 구현하고, 
// 상태 변경을 확인하여 해당 자식의 함수 ex)Enter, Tick 을 호출합니다.
// 자식의 호출함수에서 구체적인 동작을 정의합니다.
//
// PlayerState는 추상 클래스 입니다.


class PlayerState 
{

protected:
    // 상태 enum에 순서에 맞게 인덱스를 반환, 파생 클래스에서 알맞은 동작에 맞는 enum을 생성자에서 초기화
    PLAYER_STATE            m_FlipbookIndex;

    // 공유하고 있는 PlayerData와 GameObject 선언
    Ptr<CPlayerData>        m_PlayerData;

public:
    //=========
    // 멤버 함수
    //=========
    void Init();
    void ApplyGravity();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() = 0;
    virtual void Tick() = 0;
    virtual void FinalTick() = 0;
    virtual PLAYER_STATE GetFlipbookIndex() = 0;

    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;
    // Deep-copy 지원을 위한 가상 클론 함수 추가
    virtual unique_ptr<PlayerState> Clone() const = 0;


    //============
    // 생성, 소멸자
    //============
    PlayerState(Ptr<CPlayerData> _Data);
    virtual ~PlayerState();

};
