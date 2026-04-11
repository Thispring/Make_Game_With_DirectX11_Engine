#pragma once
#include "CScript.h"
#include "GameObject.h"
#include "APrefab.h"

// Script를 상속받는 콘텐츠 스크립트 Component 입니다.
// Player의 체력, 공격력 등의 정보를 이 객체에서 정의하고 관리합니다.
// 멤버는 GET_SET 함수로만 접근을 허용합니다.
class CPlayerData :
    public CScript
{

private:
    GameObject*         m_TargetObject;
    Ptr<GameObject>     m_AnchorObject;
    Ptr<APrefab>        m_EnergyBlast;      // Player가 발사하는 에너지파 프리팹
    
    Vec3                m_OriginPos;
    Vec3                m_CurPos;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    float               m_Speed;
    float               m_JumpVelocity;

    int                 m_DeathCount;
    int                 m_Direction;

    bool                m_IsDead;
    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT 만큼 음수 이동
    
    bool                m_IsAttack;         // 공격 중인지 여부, 공격 상태는 1가지만 가능합니다.
    bool                m_IsJumping;

    //===============================
    // 물리 관련 멤버 변수, 파일로 저장 X
    //===============================
    float               m_VelocityY;        // Y축 수직 속도
    Vec3                m_GroundNormal;      // 현재 접지면 법선 (기본값: 평지 = 0,1,0)
    float               m_fCoyoteTimer;     // 코요테 타임 카운터 (내리막 이탈 유예)
    int                 m_GroundContactCount;

    // ─── 벽 접촉 관련 ───
    int                 m_WallContactLeft;   // 왼쪽 벽 접촉 카운트
    int                 m_WallContactRight;  // 오른쪽 벽 접촉 카운트

public:
    //=========
    // 멤버 함수
    //=========

    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;       // Component 클래스 추상 멤버
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CPlayerData);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(float, FullHP);
    GET_SET(float, CurHP);
    GET_SET(float, Damage);
    GET_SET(float, Speed);
    GET_SET(float, JumpVelocity);

    GET_SET(int, DeathCount);
    GET_SET(int, Direction);

    GET_SET(bool, IsDead);
    GET_SET(bool, IsFalling);
    GET_SET(bool, IsJumping);
    bool GetIsAttack();
    void SetIsAttack();
    void OffIsAttack();
    
    GET_SET(Vec3, OriginPos);
    GET_SET(Vec3, CurPos);

    GET_SET(GameObject*, TargetObject);
    GET_SET(Ptr<GameObject>, AnchorObject);
    GET_SET(Ptr<APrefab>, EnergyBlast);

    GET_SET(float, VelocityY);
    GET_SET(Vec3, GroundNormal);
    GET_SET(float, fCoyoteTimer);
    GET_SET(int, GroundContactCount);

    // ─── 벽 접촉 조회 ───
    bool GetIsBlockedLeft()  const { return m_WallContactLeft > 0; }
    bool GetIsBlockedRight() const { return m_WallContactRight > 0; }

    //============
    // 생성, 소멸자
    //============
    CPlayerData();
    virtual ~CPlayerData();
};
