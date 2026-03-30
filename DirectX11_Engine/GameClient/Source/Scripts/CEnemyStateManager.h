#pragma once
#include "CScript.h"
#include "CEnemyData.h"
#include "Source\Content\EnemyState.h"
#include "contentEnum.h"

// Enemy의 상태를 정의합니다.
// Animator에서 상태를 읽어와 알맞은 Flipbook을 재생하는데 사용
class CEnemyStateManager :
    public CScript
{

private:
    Ptr<CEnemyData>                        m_EnemyData;

    EnemyState* m_CurStatus;
    EnemyState* m_PrevStatus;
    vector<unique_ptr<EnemyState>>        m_vecStatus;

    

public:
    void SetUp();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyStateManager);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    
    //============
    // 생성, 소멸자
    //============
    CEnemyStateManager();
    // unique_ptr 멤버는 디폴트 복사 생성자로, 복사 불가능
    // 복사 생성자 따로 구현
    CEnemyStateManager(const CEnemyStateManager& _Origin);
    virtual ~CEnemyStateManager();
};
