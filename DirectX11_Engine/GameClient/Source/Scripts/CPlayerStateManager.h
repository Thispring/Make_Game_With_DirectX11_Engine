#pragma once
#include "CScript.h"
#include "CPlayerData.h"
#include "Source\Content\PlayerStatus.h"
// CPlayerStatus의 주소를 받아, 상태를 관리하는 클래스입니다.

// vector 인덱스에 사용할 enum 정의
enum class PLAYER_STATE
{
    IDLE,
    MOVE,
    JUMP,
    ATTACK,

    END,
};

class CPlayerStateManager :
    public CScript
{

private:
    Ptr<CPlayerData>                        m_PlayerData;   // 관리자 클래스에서 동일한 CPlayerData를 가리키고 있어야 합니다.

    /**************************************************************************
    * PlayerStatus에 스마트 포인터를 사용한다면, 엔진에 구현된 Ptr.h가 아닌
    * std unique_ptr를 사용합니다. (콘텐츠용도로 만들었기에 엔진 차원 Entity 상속 X)
    **************************************************************************/
    PlayerStatus*                           m_CurStatus;
    PlayerStatus*                           m_PrevStatus;
    vector<unique_ptr<PlayerStatus>>        m_vecStatus;

    static int m_ChangeCount;
    bool                                    m_IsStateChange;

public:
    void ChangeState();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerStateManager);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(PlayerStatus*, CurStatus);  // Get, Set을 통해서만 Controller에서 상태 변경
    GET_SET(PlayerStatus*, PrevStatus);
    PlayerStatus* GetStatusVec(int _Idx) { return m_vecStatus[_Idx].get(); }
    void SetStateChange() { m_IsStateChange = true; }
    bool IsStateChange();
    GET_SET(int, ChangeCount);


    //============
    // 생성, 소멸자
    //============
    CPlayerStateManager();
    // unique_ptr 멤버는 디폴트 복사 생성자로, 복사 불가능
    // 복사 생성자 따로 구현
    CPlayerStateManager(const CPlayerStateManager& _Origin);
    virtual ~CPlayerStateManager();
};
