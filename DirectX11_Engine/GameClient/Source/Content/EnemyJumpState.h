#pragma once
#include "EnemyState.h"

class EnemyJumpState 
    : public EnemyState
{

public:
    //=============
    // 상속 멤버 함수 (훅)
    //=============
    void OnBegin() override;
    void OnTick() override;
    void OnFinalTick() override;

    void SaveToLevelFile(FILE* _File) override;
    void LoadFromLevelFile(FILE* _File) override;
    unique_ptr<EnemyState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    EnemyJumpState(Ptr<CEnemyData> _Data);
    virtual ~EnemyJumpState();
};
