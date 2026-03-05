#pragma once
#include "Component.h"
#include "Source/ScriptMgr.h"

class CCollider2D;

class CScript :
    public Component
{

private:
    int     m_ScriptType;   // enum 값으로 초기화

protected:
    void Destroy(); // 스크립트 컴포넌트의 오브젝트 삭제 요청 함수

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() {}
    virtual void Tick() = 0;
    virtual void FinalTick() final{}    // final 키워드에 대해 알아보기
    virtual CScript* Clone() = 0;


    //=========
    // Get, Set
    //=========
    int GetScriptType() { return m_ScriptType; }


    //============
    // 생성, 소멸자
    //============
    CScript(int _ScriptType);
    virtual ~CScript();
};

// 자주 사용되는 OverLap 관련 기능 재정의
#define ADD_DYNAMIC_BEGIN_OVERLAP(MemFunc) Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_OVERLAP(MemFunc) Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_END_OVERLAP(MemFunc) Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&MemFunc);
