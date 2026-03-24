#pragma once
#include "CScript.h"
#include "CPlayerData.h"
#include "Source\Content\PlayerStatus.h"


class CPlayerController :
    public CScript
{

private:
    Ptr<CPlayerData>    m_PlayerData;   // 관리자 클래스에서 동일한 CPlayerData를 가리키고 있어야 합니다.

    // 현재 상태
    PlayerStatus*                    m_CurStatus;
    // Player 상태 목록
    vector<PlayerStatus*>            m_vecStatus;

public:
    //=========
    // 멤버 함수
    //=========


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
