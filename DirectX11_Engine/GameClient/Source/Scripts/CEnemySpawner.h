#pragma once
#include "CScript.h"

#include "APrefab.h"
#include "GameObject.h"

//=======================
// Enemy 소환을 관리합니다.
//=======================
class CEnemySpawner :
    public CScript
{

private:
    // ImGui로 Prefab을 등록하기 위해, Type 수 만큼 멤버를 선언합니다.
    Ptr<APrefab>                m_Demon;
    Ptr<APrefab>                m_Skull;
    Ptr<APrefab>                m_Flying;
    Ptr<APrefab>                m_Flower;
    Ptr<APrefab>                m_Boss;


    vector<Ptr<GameObject>>     m_vecSpawnEnemy;        // 소환 된 Enemy를 관리하는 벡터
    int                         m_spawnCount;           // 소환 된 Enemy 총 개수
    float                       m_spawnTime;            // 스폰 시간 주기

public:
    //=========
    // 멤버 함수
    //=========
    void SpawnEnemy();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemySpawner);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CEnemySpawner();
    virtual ~CEnemySpawner();
};
