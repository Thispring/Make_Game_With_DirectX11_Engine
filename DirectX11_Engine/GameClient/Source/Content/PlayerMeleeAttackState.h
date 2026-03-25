#pragma once
#include "PlayerState.h"

/********************************************
* Player의 근접 공격에 대해 정의합니다.
* 근접 공격에서 세부동작은, 해당 클래스 파일에서 
* 이에 알맞은 파생클래스를 만들어서 관리합니다.
********************************************/
#pragma region PlayerMeleeAttackState
class PlayerMeleeAttackState :
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
    PlayerMeleeAttackState();
    virtual ~PlayerMeleeAttackState();
};
#pragma endregion


#pragma region PlayerPunchState
class PlayerPunchState :
    public PlayerMeleeAttackState
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
    PlayerPunchState();
    virtual ~PlayerPunchState();
};
#pragma endregion


#pragma region PlayerMiddleKickState
class PlayerMiddleKickState :
    public PlayerMeleeAttackState
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
    PlayerMiddleKickState();
    virtual ~PlayerMiddleKickState();
};
#pragma endregion


#pragma region PlayerHighKickState
class PlayerHighKickState :
    public PlayerMeleeAttackState
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
    PlayerHighKickState();
    virtual ~PlayerHighKickState();
};
#pragma endregion
