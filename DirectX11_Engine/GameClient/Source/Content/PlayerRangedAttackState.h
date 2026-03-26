#pragma once
#include "PlayerState.h"

#pragma region PlayerRangedAttackState
class PlayerRangedAttackState :
    public PlayerState
{

private:

public:

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual PLAYER_STATE GetFlipbookIndex() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    // Deep-copy 지원을 위한 가상 클론 함수 추가
    virtual unique_ptr<PlayerState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    PlayerRangedAttackState();
    virtual ~PlayerRangedAttackState();
};
#pragma endregion


#pragma region PlayerChargeState
class PlayerChargeState :
    public PlayerRangedAttackState
{

private:

public:

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual PLAYER_STATE GetFlipbookIndex() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    // Deep-copy 지원을 위한 가상 클론 함수 추가
    virtual unique_ptr<PlayerState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    PlayerChargeState();
    virtual ~PlayerChargeState();
};
#pragma endregion


#pragma region PlayerReleaseState
class PlayerReleaseState :
    public PlayerRangedAttackState
{

private:

public:

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual PLAYER_STATE GetFlipbookIndex() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    // Deep-copy 지원을 위한 가상 클론 함수 추가
    virtual unique_ptr<PlayerState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    PlayerReleaseState();
    virtual ~PlayerReleaseState();
};
#pragma endregion
