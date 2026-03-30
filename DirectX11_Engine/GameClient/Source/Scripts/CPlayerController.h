#pragma once
#include "CScript.h"
#include "CPlayerData.h"
#include "CPlayerStateManager.h"
#include "Source\Content\PlayerState.h"


class CPlayerController :
    public CScript
{

private:
    Ptr<CPlayerData>            m_PlayerData;   // 관리자 클래스에서 동일한 CPlayerData를 가리키고 있어야 합니다.
    Ptr<CPlayerStateManager>    m_StatusMgr;
    int m_TempDir;

public:
    //=========
    // 멤버 함수
    //=========
    void Move();
    void Jump();
    void Punch();
    void Kick();
    void EnergyBlastShot();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CPlayerController();
    virtual ~CPlayerController();
};
