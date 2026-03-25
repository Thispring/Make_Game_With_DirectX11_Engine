#pragma once
#include "PlayerState.h"

// NOTE(26-03-25):
// MoveState는 현재 Walk 기능만 수행,
// 추후 이동 방식이 다양화 된다면, 해당 클래스 파일에서
// 파생 클래스들을 생성
class PlayerMoveState :
    public PlayerState
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual PLAYER_STATE GetFlipbookIndex() override;
    virtual unique_ptr<PlayerState> Clone() const override;


    //============
    // 생성, 소멸자
    //============  
    PlayerMoveState();
    virtual ~PlayerMoveState();
};

