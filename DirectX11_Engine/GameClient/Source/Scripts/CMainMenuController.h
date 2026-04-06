#pragma once
#include "CScript.h"
#include "KeyMgr.h"

class CMainMenuController :
    public CScript
{

private:
    float       m_Time;

    KEY         m_StartKey;
    KEY         m_OptionKey;
    KEY         m_CreditKey;

public:
    void Start(KEY _Key);
    void Option(KEY _Key);
    void Credit(KEY _Key);

    void ShuffleKey();
    void ChangeKeyUI();

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CMainMenuController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CMainMenuController();
    virtual ~CMainMenuController();
};

