#pragma once
#include "EnemyState.h"

#pragma region EnemyAttackState (근접)
class EnemyAttackState 
    : public EnemyState
{

private:
    bool    m_bInterrupted;     // TakeDamage 이벤트 수신 시 공격 중단 플래그


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
    EnemyAttackState(Ptr<CEnemyData> _Data);
    virtual ~EnemyAttackState();
};
#pragma endregion


#pragma region EnemyRangedAttack (원거리 공격)
class EnemyRangedAttackState
    : public EnemyState
{

private:
    float   m_fireTime;

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
    EnemyRangedAttackState(Ptr<CEnemyData> _Data);
    virtual ~EnemyRangedAttackState();
};
#pragma endregion
