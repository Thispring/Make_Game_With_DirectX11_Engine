#pragma once
#include "EnemyState.h"

class EnemyIdleState 
    : public EnemyState
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    // NOTE(26-04-01):
    // 이제 부모의 non-virtual 진입점을 사용하므로 On* 훅만 구현
    void OnBegin() override;
    void OnTick() override;
    void OnFinalTick() override;

    void SaveToLevelFile(FILE* _File) override;
    void LoadFromLevelFile(FILE* _File) override;
    unique_ptr<EnemyState> Clone() const override;


    //============
    // 생성, 소멸자
    //============
    EnemyIdleState(Ptr<CEnemyData> _Data);
    virtual ~EnemyIdleState();
};
