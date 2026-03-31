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
    /*******************************************************
    * PlayerMeleeAttackState도 부모 클래스의 역할을 하기 때문에
    * 멤버함수 오버라이딩 구현 X
    *******************************************************/

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
    PlayerMeleeAttackState(Ptr<CPlayerData> _Data);
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
    PlayerPunchState(Ptr<CPlayerData> _Data);
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
    PlayerMiddleKickState(Ptr<CPlayerData> _Data);
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
    PlayerHighKickState(Ptr<CPlayerData> _Data);
    virtual ~PlayerHighKickState();
};
#pragma endregion


#pragma region PlayerLowKickState
class PlayerLowKickState :
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
    PlayerLowKickState(Ptr<CPlayerData> _Data);
    virtual ~PlayerLowKickState();
};
#pragma endregion
