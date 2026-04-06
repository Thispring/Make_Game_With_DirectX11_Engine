#pragma once
#include "EnemyState.h"

class EnemyPatrolState :
    public EnemyState
{

private:
    float       m_flowerPatrolTime;

public:
    //============= 
    // 상속 멤버 함수
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
    EnemyPatrolState(Ptr<CEnemyData> _Data);
    virtual ~EnemyPatrolState();
};
