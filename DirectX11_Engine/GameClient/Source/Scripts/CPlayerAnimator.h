#pragma once
#include "CScript.h"
#include "CPlayerStateManager.h"

class CPlayerAnimator :
    public CScript
{

private:
    bool                        m_IsPlaying;        // FlipBook 재생이 1회 끝났는지 여부
    Ptr<CPlayerStateManager>    m_StatusMgr;

    // 이전 프레임 공격 키 입력 상태 (Flipbook 종료 시점 판단용)
    bool                        m_bPrevPunchKey;    // Z 키 (펀치)
    bool                        m_bPrevKickKey;     // X 키 (킥)

    bool                        m_bPlayStarted;     // 첫 Tick에서 초기 Idle 애니메이션 재생 보장

public:
    //=========
    // 멤버 함수
    //=========
    void Play();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerAnimator);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CPlayerAnimator();
    virtual ~CPlayerAnimator();
};
