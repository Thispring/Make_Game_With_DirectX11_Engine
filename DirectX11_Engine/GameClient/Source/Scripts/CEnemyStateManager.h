#pragma once
#include "CScript.h"
#include "CEnemyData.h"
#include "Source\Content\EnemyState.h"
#include <map>

// Enemy의 상태를 정의합니다.
// Animator에서 상태를 읽어와 알맞은 Flipbook을 재생하는데 사용
class CEnemyStateManager :
    public CScript
{

private:
    Ptr<CEnemyData>                                     m_EnemyData;

    EnemyState*                                         m_CurStatus;
    EnemyState*                                         m_PrevStatus;
    // 상태 컨테이너를 인덱스 기반 vector에서 키 기반 map으로 변경
    //map<ENEMY_STATE, unique_ptr<EnemyState>>     m_mapStatus;
    map<ENEMY_STATE, pair<unique_ptr<EnemyState>, int>> m_mapStatus;

    bool                                                m_IsChange;

public:
    //=========
    // 멤버 함수
    //=========
    void SetUp();
    void ChangeState();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyStateManager);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    void SetChange() { m_IsChange = true; }
    bool IsChange();
    GET_SET(EnemyState*, CurStatus); 
    GET_SET(EnemyState*, PrevStatus);
    
    map<ENEMY_STATE, pair<unique_ptr<EnemyState>, int>>& GetStateMap() { return m_mapStatus; }

    // 기존 인덱스 기반 접근 대신 enum 키로 상태 획득
    EnemyState* GetStatusByCommonState(ENEMY_STATE _State);

    // 호환성: 기존 코드에서 int 인덱스(ENEMY_STATE값)를 사용하므로
    // 그대로 호출 가능한 멤버를 유지합니다.
    EnemyState* GetStatusByIndex(int _Idx);

    // 현재 상태의 ENEMY_STATE 키를 반환
    ENEMY_STATE GetCurCommonState();

    int GetFlipBookIndex();


    //============
    // 생성, 소멸자
    //============
    CEnemyStateManager();
    // unique_ptr 멤버는 디폴트 복사 생성자로, 복사 불가능
    // 복사 생성자 따로 구현
    CEnemyStateManager(const CEnemyStateManager& _Origin);
    virtual ~CEnemyStateManager();

};
