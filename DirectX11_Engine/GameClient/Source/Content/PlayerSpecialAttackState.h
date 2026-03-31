#pragma once
#include "PlayerState.h"

#pragma region PlayerSpecialAttackState
class PlayerSpecialAttackState :
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
    PlayerSpecialAttackState(Ptr<CPlayerData> _Data);
    virtual ~PlayerSpecialAttackState();
};

#pragma endregion

