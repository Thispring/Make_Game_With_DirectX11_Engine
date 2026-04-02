#pragma once
#include "Source\Scripts\CEnemyData.h"
#include <string>

/*****************************************************************************************************
* 1. CScript 비상속 이유: 
*    - Begin/Tick 등 기존 메서드 이름 재사용 및 일관성 유지.
*    - 레벨 로드 시점이 아닌, 실제 게임 플레이 시점에만 정확히 구동하기 위함.
* 
* 2. 충돌 처리 관련 (To-do):
*    - 현재 Collider 콜백이 CScript 기반으로 설계되어 있어 임시로 Data 클래스(CScript 상속)에서 처리 중.
*    - 추후 상태 클래스에서 직접 충돌 검사를 수행하도록 구조 개선 필요.
*****************************************************************************************************/
class EnemyState
{

protected:
    // 상태 enum에 순서에 맞게 인덱스를 반환, 파생 클래스에서 알맞은 동작에 맞는 enum을 생성자에서 초기화
    ENEMY_STATE             m_FlipbookIndex;

    // Enemy의 정보가 담겨져있는 클래스 입니다.
    // 생성 시점에 오브젝트 이름을 받아, 해당 오브젝트에 설정된 Data 정보를 가져옵니다.
    Ptr<CEnemyData>         m_EnemyData;

    // 생성자에서 전달된 소유자(오브젝트) 이름
    wstring                 m_OwnerName;


    //=============
    // 상속 멤버 함수
    //=============
    virtual void OnBegin() = 0;
    virtual void OnTick() = 0;
    virtual void OnFinalTick() = 0;

public:
    //=========
    // 멤버 함수
    //=========
    void ApplyGravity();
    // 상태 진입 시, EnemyData 멤버인 TimeInState를 초기화, 계산하는 함수
    // 파생클래스의 Begin에서 초기화 함수 호출, Tick에서 계산하는 함수 호출
    void CalTimeInState();
    void ClearTimeInState();


    //==========================================================
    // 상태 진입/실행/종료 - 템플릿 메서드
    // 부모가 공통 로직을 보장하도록 non-virtual 진입점으로 구현합니다.
    //==========================================================
    void Begin();
    void Tick();
    void FinalTick();


    //=============
    // 상속 멤버 함수
    //=============
    virtual ENEMY_STATE GetFlipbookIndex() = 0;

    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;
    virtual unique_ptr<EnemyState> Clone() const = 0;


    //============
    // 생성, 소멸자
    //============
    /***************************************************************
    * NOTE(26-03-31):
    * 
    * EnemyState의 기본 생성자는 사용하지 않습니다.
    * 추후, Level 시작 전에 의도치 않게 생성자가 호출이 되는 현상이 있다면
    * 기본 생성자를 만들어서 디버깅에 사용합니다.
    ***************************************************************/
    // EnemyData 포인터를 받아 초기화하는 생성자
    EnemyState(Ptr<CEnemyData> _Data);
    virtual ~EnemyState();
};
