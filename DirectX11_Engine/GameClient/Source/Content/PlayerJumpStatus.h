#pragma once
#include "PlayerStatus.h"
class PlayerJumpStatus :
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
    virtual unique_ptr<PlayerStatus> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    PlayerJumpStatus();
    virtual ~PlayerJumpStatus();
};

