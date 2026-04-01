#pragma once
#include "Component.h"
#include "CScript.h"

class CCollider2D;
typedef void(CScript::* COLLISION_EVENT)(CCollider2D*, CCollider2D*);

// 스크립트에서 발생한 충돌 이벤트
struct COLLISION_DELEGATE
{
    CScript*        Inst;
    COLLISION_EVENT MemFunc;
};


class CCollider2D :
    public Component
{

private:
    COLLIDER2D_SHAPE m_Shape;       // 충돌체 모양
    float            m_HalfAngle;   // 부채꼴용 반각 (라디안)
    Vec3             m_WorldCenter; // 월드 기준 중심(혹은 부채꼴 팁)
    Vec3             m_WorldDir;    // 월드 기준 방향 (부채꼴용)
    float            m_WorldRadius; // 월드 기준 스케일값 (반지름 역할)

    Vec2    m_Scale;        // 충돌체의 크기
    Vec2    m_Offset;       // 충돌체가 부모 오브젝트와 얼마나 떨어져 있는지
    Matrix  m_matWorld;
    int     m_OverlapCount; // 충돌에 대한 카운트
    bool    m_Enabled;      // 콜라이더 활성화 여부 (충돌 검사에 사용)

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

    void SetEnabled(bool _Enable) { m_Enabled = _Enable; }
    bool IsEnabled() const { return m_Enabled; }


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

    COLLIDER2D_SHAPE GetShape() const { return m_Shape; }
    void             SetShape(COLLIDER2D_SHAPE _Shape) { m_Shape = _Shape; }

    float GetHalfAngle() const { return m_HalfAngle; }
    void  SetHalfAngle(float _HalfAngle) { m_HalfAngle = _HalfAngle; }

    Vec3  GetWorldCenter() const { return m_WorldCenter; }
    Vec3  GetWorldDir() const { return m_WorldDir; }
    float GetWorldRadius() const { return m_WorldRadius; }


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
