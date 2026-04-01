#pragma once
#include "EnemyState.h"

/******************************************************
* EnemyDamageState는 피해를 입었을 때의 상태를 관리합니다. 
* ex) 체력 상태에 따라 피격 -> 사망 으로 이어지게
* EnemyDamageState는 부모 클래스로 두고
* 이를 파생하여, Hit와 Dead 클래스 구현
******************************************************/
#pragma region EnemyDamageState
class EnemyDamageState :
    public EnemyState
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    // DamageState는 구체적 훅을 그대로 노출하지 않고
    // 기본 클래스의 훅을 따릅니다.
    virtual ENEMY_STATE GetFlipbookIndex() = 0;

    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;
    virtual unique_ptr<EnemyState> Clone() const = 0;

    //============
    // 생성, 소멸자
    //============
    EnemyDamageState(Ptr<CEnemyData> _Data);
    virtual ~EnemyDamageState();
};
#pragma endregion


#pragma region EnemyHitState
class EnemyHitState :
    public EnemyDamageState
{

public:

    //=============
    // 상속 멤버 함수
    //=============
    virtual void OnBegin() override;
    virtual void OnTick() override;
    virtual void OnFinalTick() override;
    virtual ENEMY_STATE GetFlipbookIndex() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    virtual unique_ptr<EnemyState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    EnemyHitState(Ptr<CEnemyData> _Data);
    virtual ~EnemyHitState();
};
#pragma endregion


#pragma region EnemyDeadState
class EnemyDeadState :
    public EnemyDamageState
{


public:
    //=============
    // 상속 멤버 함수 (훅)
    //=============
    virtual void OnBegin() override;
    virtual void OnTick() override;
    virtual void OnFinalTick() override;
    virtual ENEMY_STATE GetFlipbookIndex() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    virtual unique_ptr<EnemyState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    EnemyDeadState(Ptr<CEnemyData> _Data);
    virtual ~EnemyDeadState();
};
#pragma endregion
