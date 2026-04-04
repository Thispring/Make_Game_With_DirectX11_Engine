#pragma once
#include "CScript.h"
#include "CPlayerData.h"
#include "CPlayerStateManager.h"
#include "Source\Content\PlayerState.h"

#include "KeyMgr.h"

class CPlayerController :
    public CScript
{

private:
    Ptr<CPlayerData>            m_PlayerData;   // 관리자 클래스에서 동일한 CPlayerData를 가리키고 있어야 합니다.
    Ptr<CPlayerStateManager>    m_StatusMgr;
    int                         m_TempDir;

    KEY                         m_PunchKey;
    KEY                         m_KickKey;
    KEY                         m_BlastShotKey;

    bool                        m_bHasDied;         // 최초 사망 이후 랜덤 키 모드 진입
    bool                        m_bKeyRevealed[3];  // 각 공격 키 발견 여부 (0=Punch, 1=Kick, 2=BlastShot)

public:
    //=========
    // 멤버 함수
    //=========
    void Move();
    void Jump();
    //==================================================
    // NOTE(26-04-04): 공격 함수 KEY를 무작위로 
    // 설정하기 위해 매개변수 추가
    //==================================================
    void Punch(KEY _key);
    void Kick(KEY _key);
    void EnergyBlastShot(KEY _key);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(KEY, PunchKey);
    GET_SET(KEY, KickKey);
    GET_SET(KEY, BlastShotKey);

    bool HasDied() const { return m_bHasDied; }
    void SetHasDied(bool _b) { m_bHasDied = _b; }
    bool IsKeyRevealed(int _idx) const { return m_bKeyRevealed[_idx]; }
    void ResetKeyRevealed();


    //============
    // 생성, 소멸자
    //============
    CPlayerController();
    virtual ~CPlayerController();
};
