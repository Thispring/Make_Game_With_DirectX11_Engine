#pragma once
#include "CScript.h"

class CSavePoint :
    public CScript
{

private:
    Vec3    m_InitialPos;
    Vec3    m_OriginPos;
    float   m_BobTime;      // DT를 누적한 경과 시간 (PLAY 중에만 증가)
    bool    m_bTriggered;   // BeginOverlap 이후 소멸 애니메이션 재생 중 여부

    static constexpr float BOB_SPEED = 2.0f;        // 라디안/초 (2π ≈ 6.28 = 1회 왕복/초)
    static constexpr float BOB_AMPLITUDE = 15.0f;   // 위아래 진폭 (단위: 게임 좌표)

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
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CSavePoint);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CSavePoint();
    virtual ~CSavePoint();
};
