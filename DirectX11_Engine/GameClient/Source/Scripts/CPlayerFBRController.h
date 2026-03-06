#pragma once
#include "CScript.h"
#include "CPlayerStatus.h"

// Player의 FlipBookRender의 상태를 관리하는 클래스 입니다.
class CPlayerFBRController :
    public CScript
{

private:
    bool                    m_IsPlaying;       // FlipBook 재생이 1회 끝났는지 여부, false이면 아직 남아있는 Sprite가 있고, true를 얻어오면 1회 재생 끝
    Ptr<CPlayerStatus>      m_Target;          // FlipBook 재생 대상 오브젝트
    PLAYER_STATE            m_PrevState;       // 이전 상태 저장
    PLAYER_STATE            m_CurState;        // 현재 상태 저장

public:
    //=========
    // 멤버 함수
    //=========
    void CheckState();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerFBRController);

    
    //============
    // 생성, 소멸자
    //============
    CPlayerFBRController();
    virtual ~CPlayerFBRController();

};
