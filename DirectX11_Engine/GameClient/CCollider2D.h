#pragma once
#include "Component.h"
#include "CScript.h"

class CCollider2D;
typedef void(CScript::* COLLISION_EVENT)(CCollider2D*, CCollider2D*);

struct COLLISION_DELEGATE
{
    CScript*        Inst;
    COLLISION_EVENT MemFunc;
};


class CCollider2D :
    public Component
{

private:
    Vec2    m_Scale;        // 충돌체의 크기
    Vec2    m_Offset;       // 충돌체가 부모 오브젝트와 얼마나 떨어져 있는지
    Matrix  m_matWorld;
    int     m_OverlapCount; // 충돌에 대한 카운트

    vector<COLLISION_DELEGATE>  m_vecBeginDel;
    vector<COLLISION_DELEGATE>  m_vecOverDel;
    vector<COLLISION_DELEGATE>  m_vecEndDel;

    //=================
    // private 멤버 함수
    //=================
    // Overlap이란?
    void BeginOverlap(Ptr<CCollider2D> _Other);
    void Overlap(Ptr<CCollider2D> _Other);
    void EndOverlap(Ptr<CCollider2D> _Other);

public:
    //=========
    // 멤버 함수
    //=========
    // CScript를 가진 GameObject의 Begin에서 아래 함수를 호출하여,
    // 충돌 검사 대상에 등록합니다.
    void AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
    void AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
    void AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    CLONE(CCollider2D);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(Vec2, Scale);
    GET_SET(Vec2, Offset);
    const Matrix& GetWorldMat() { return m_matWorld; }


    //============
    // 생성, 소멸자
    //============
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    virtual ~CCollider2D();


    //=============
    // friend class
    //=============
    friend class CollisionMgr;
};
