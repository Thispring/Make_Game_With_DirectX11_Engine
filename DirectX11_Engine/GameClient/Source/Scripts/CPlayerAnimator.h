#pragma once
#include "CScript.h"
#include "Source\Content\PlayerStatus.h"

// Player의 상태를 읽어와, 적절한 Flipbook을 재생시키는
// 애니메이션 관리 클래스 입니다.
class CPlayerAnimator :
    public CScript
{

private:
    bool                    m_IsPlaying;       // FlipBook 재생이 1회 끝났는지 여부, false이면 아직 남아있는 Sprite가 있고, true를 얻어오면 1회 재생 끝
    PlayerStatus*           m_Target;          // FlipBook 재생 대상 오브젝트

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
    CLONE(CPlayerAnimator);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    
    //============
    // 생성, 소멸자
    //============
    CPlayerAnimator();
    virtual ~CPlayerAnimator();

};
