#pragma once
#include "CScript.h"
#include "CEnemyData.h"

class CEnemyEyes :
    public CScript
{

private:
    Ptr<CEnemyData>         m_EnemyData;
    // 마지막으로 플레이어를 감지한 이후 경과 시간 (초)
    float                   m_fTimeSinceLastDetect;
    // 플레이어 미감지 시 IDLE로 전환하기 위한 그레이스 타임(초)
    float                   m_fDetectGraceTime;
    // 플레이어가 최근에 감지되었는지 플래그
    bool                    m_bPlayerRecentlyDetected;

public:
    //=========
    // 멤버 함수
    //=========
    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyEyes);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CEnemyEyes();
    virtual ~CEnemyEyes();
};

