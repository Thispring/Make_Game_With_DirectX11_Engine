#pragma once
#include "CScript.h"
#include "CEnemyStateManager.h"
#include "CEnemyData.h"

class CEnemyAnimator :
    public CScript
{

private:
    bool                        m_IsPlaying;        // FlipBook 재생이 1회 끝났는지 여부, false이면 아직 남아있는 Sprite가 있고, true를 얻어오면 1회 재생 끝
    Ptr<CEnemyStateManager>     m_StatusMgr;        // 상태 매니저의 원본 주소를 가져와, 현재 상태를 읽어와 알맞은 Flipbook을 재생합니다.
    Ptr<CEnemyData>             m_EnemyData;

public:
    //=========
    // 멤버 함수
    //=========
    void Play();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyAnimator);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CEnemyAnimator();
    virtual ~CEnemyAnimator();
};

