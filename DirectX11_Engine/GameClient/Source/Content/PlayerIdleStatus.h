#pragma once
#include "PlayerStatus.h"

class PlayerIdleStatus :
    public PlayerStatus
{
    // 상수 멤버로 Flipbook enum값 저장

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
    virtual unique_ptr<PlayerStatus> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    PlayerIdleStatus();
    virtual ~PlayerIdleStatus();
};

