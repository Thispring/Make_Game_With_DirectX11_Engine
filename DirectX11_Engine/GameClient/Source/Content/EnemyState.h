#pragma once
#include "Source\Scripts\CEnemyData.h"
#include "contentEnum.h"

class EnemyState
{

protected:
    // 상태 enum에 순서에 맞게 인덱스를 반환, 파생 클래스에서 알맞은 동작에 맞는 enum을 생성자에서 초기화
    ENEMY_STATE            m_FlipbookIndex;

    // 공유되는 GameObject 선언
    Ptr<CEnemyData>        m_EnemyData;

public:
    //=========
    // 멤버 함수
    //=========
    void ApplyGravity();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() = 0;
    virtual void Tick() = 0;
    virtual void FinalTick() = 0;
    virtual ENEMY_STATE GetFlipbookIndex() = 0;

    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;
    virtual unique_ptr<EnemyState> Clone() const = 0;


    //============
    // 생성, 소멸자
    //============
    EnemyState();
    virtual ~EnemyState();

};
