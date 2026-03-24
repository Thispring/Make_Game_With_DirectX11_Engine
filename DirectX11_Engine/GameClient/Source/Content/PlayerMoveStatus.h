#pragma once
#include "PlayerStatus.h"

// 구체적인 이동 방식에 대해 정의합니다.
enum class MOVEMENT_STATE
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    JUMP,

};

class PlayerMoveStatus :
    public PlayerStatus
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
    virtual int GetFlipbookIndex() override;


    //============
    // 생성, 소멸자
    //============  
    PlayerMoveStatus();
    virtual ~PlayerMoveStatus();
};

