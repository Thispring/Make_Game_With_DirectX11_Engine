#pragma once
#include "EnemyState.h"

class EnemyAttackState 
    : public EnemyState
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    void Begin() override;
    void Tick() override;
    void FinalTick() override;
    ENEMY_STATE GetFlipbookIndex() override;

    void SaveToLevelFile(FILE* _File) override;
    void LoadFromLevelFile(FILE* _File) override;
    unique_ptr<EnemyState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    EnemyAttackState();
    virtual ~EnemyAttackState();
};
