#pragma once
#include "CScript.h"
#include "CPlayerData.h"

// 근접 공격 관련 트리거 스크립트
// Player 근접 공격 시, Collider 충돌과 데미지 판정을 처리합니다.
class CPlayerMeleeTrigger :
    public CScript
{

private:
    Ptr<CPlayerData>    m_PlayerData;

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
    CLONE(CPlayerMeleeTrigger);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CPlayerMeleeTrigger();
    virtual ~CPlayerMeleeTrigger();
};
