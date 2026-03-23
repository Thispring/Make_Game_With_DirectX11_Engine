#pragma once
#include "CScript.h"
#include "Source\Content\PlayerStatus.h"

// CPlayerStatus의 주소를 받아, 상태를 관리하는 클래스입니다.

class CPlayerStateManager :
    public CScript
{

private:
    PlayerStatus*       m_Status;

public:


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerStateManager);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CPlayerStateManager();
    virtual ~CPlayerStateManager();
};
