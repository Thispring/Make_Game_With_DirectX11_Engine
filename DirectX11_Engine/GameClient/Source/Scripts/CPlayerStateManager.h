#pragma once
#include "CScript.h"
#include "CPlayerData.h"
#include "Source\Content\PlayerState.h"
// CPlayerStatus의 주소를 받아, 상태를 관리하는 클래스입니다.

class CPlayerStateManager :
    public CScript
{

private:
    Ptr<CPlayerData>                                m_PlayerData;   // 관리자 클래스에서 동일한 CPlayerData를 가리키고 있어야 합니다.

    /**************************************************************************
    * PlayerState에 스마트 포인터를 사용한다면, 엔진에 구현된 Ptr.h가 아닌
    * std unique_ptr를 사용합니다. (콘텐츠용도로 만들었기에 엔진 차원 Entity 상속 X)
    **************************************************************************/
    PlayerState*                                    m_CurStatus;
    PlayerState*                                    m_PrevStatus;

    // NOTE(26-04-03):
    // enemy와 동일하게 map 자료형을 변경하기
    map<PLAYER_STATE, unique_ptr<PlayerState>>      m_mapStatus;

    bool                                            m_IsChange;

public:
    void ChangeState();
    void TakeDamage(float _Damage);
    void Respawn();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerStateManager);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(PlayerState*, CurStatus);  // Get, Set을 통해서만 Controller에서 상태 변경
    GET_SET(PlayerState*, PrevStatus);
    PlayerState* GetStatusByPlayerState(PLAYER_STATE _State)
    {
        auto it = m_mapStatus.find(_State);
        return (it != m_mapStatus.end()) ? it->second.get() : nullptr;
    }

    // 호환성: 기존 인덱스 기반 접근을 사용하는 코드 지원
    PlayerState* GetStatusByIndex(int _Idx)
    {
        PLAYER_STATE st = static_cast<PLAYER_STATE>(_Idx);
        return GetStatusByPlayerState(st);
    }
    void SetChange() { m_IsChange = true; }
    bool IsChange();
    bool IsInputLocked() const
    {
        if (!m_CurStatus) return false;
        PLAYER_STATE cur = m_CurStatus->GetFlipbookIndex();
        return (cur == PLAYER_STATE::PUNCH          ||
                cur == PLAYER_STATE::MIDDLE_KICK    ||
                cur == PLAYER_STATE::HIGH_KICK      ||
                cur == PLAYER_STATE::LOW_KICK       ||
                //cur == PLAYER_STATE::JUMP           ||
                cur == PLAYER_STATE::ENERGYBLAST_SHOT);
    }


    //============
    // 생성, 소멸자
    //============
    CPlayerStateManager();
    // unique_ptr 멤버는 디폴트 복사 생성자로, 복사 불가능
    // 복사 생성자 따로 구현
    CPlayerStateManager(const CPlayerStateManager& _Origin);
    virtual ~CPlayerStateManager();
};
