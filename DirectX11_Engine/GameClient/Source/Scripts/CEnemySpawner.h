#pragma once
#include "CScript.h"

// 게임에 등장하는 Enemy 소환을 관리합니다.
class CEnemySpawner :
    public CScript
{

private:
    //vector<GameObject> m_enemyVec;      // 소환 된 Enemy를 관리하는 자료구조 벡터
    int m_spawnCount;                   // 소환 된 Enemy 총 개수
    float m_spawnTime;                  // 스폰 시간 주기

public:
    //=========
    // 멤버 함수
    //=========
    void SpawnEnemy();  // Enemy GameObject를 만들어, 레벨에 배치하는 함수


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick() override;
    virtual void Begin() override;
    CLONE(CEnemySpawner);


    //============
    // 생성, 소멸자
    //============
    CEnemySpawner();
    virtual ~CEnemySpawner();
};
