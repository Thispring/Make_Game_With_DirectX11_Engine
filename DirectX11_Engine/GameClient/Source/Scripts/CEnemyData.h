#pragma once
#include "CScript.h"
#include "GameObject.h"

/*******************************************************************************
* 게임에 등장하는 모든 Enemy의 체력, 공격력 등의 정보를 이 객체에서 정의하고 관리합니다.
* Enemy의 타입별로 멤버를 다르게 설정할 수 있도록 설계합니다.
* 멤버는 GET_SET 함수로만 접근을 허용합니다.
*******************************************************************************/

// TakeDamage 이벤트 타입 정의
using EnemyDamageEvent = function<void()>;

class CEnemyData :
    public CScript
{

private:
    GameObject*         m_TargetObject;
    GameObject*         m_EyeObject;
    Ptr<APrefab>        m_FlowerProjectile;     // FLOWER 타입만 보유하도록 설계

    ENEMY_TYPE          m_EnemyType;

    //==========================================
    // NOTE(26-04-07): 
    // 파일로 저장하여 초기 위치를 지정하는 멤버 추가
    //==========================================
    Vec3                m_InitialPos;
    Vec3                m_InitialRot;
    Vec3                m_InitialScale;

    Vec3                m_OriginPos;
    Vec3                m_CurPos;

    Vec3                m_OriginRot;
    Vec3                m_CurRot;

    float               m_FullHP;
    float               m_CurHP;
    float               m_Damage;
    
    float               m_Speed;
    float               m_JumpVelocity;
    float               m_VelocityY;        // Y축 수직 속도
    Vec3                m_GroundNormal;      // 현재 접지면 법선 (기본값: 평지 = 0,1,0)
    float               m_fCoyoteTimer;     // 코요테 타임 카운터 (내리막 이탈 유예)
    int                 m_GroundContactCount;
    int                 m_WallContactLeft;   // 왼쪽 벽 접촉 카운트
    int                 m_WallContactRight;  // 오른쪽 벽 접촉 카운트
    float               m_Offset;
    
    float               m_TimeSinceSpawn;
    float               m_TimeInState;

    int                 m_Direction;        // 이동방향

    bool                m_IsDead;
    bool                m_IsFalling;        // true일 때만 Tick에서 y축으로 DT 만큼 음수 이동
    bool                m_IsAttack;

    bool                m_isFixedDir;       // 방향 고정 여부

    EnemyDamageEvent    m_OnTakeDamageEvent;

    //=================
    // private 멤버 함수
    //=================
    void ChangeState(ENEMY_STATE _State);

public:
    //=========
    // 멤버 함수
    //=========
    void TakeDamage(float _Damage, bool _hitSkull = false);

    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    void CreateProjectile();

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;       // Component 클래스 추상 멤버
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CEnemyData);

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
    GET_SET(float, VelocityY);
    GET_SET(Vec3, GroundNormal);
    GET_SET(int, GroundContactCount);
    GET_SET(float, TimeSinceSpawn);
    GET_SET(float, TimeInState);
    GET_SET(float, Offset);

    GET_SET(int, Direction);

    GET_SET(bool, IsDead);
    GET_SET(bool, IsFalling);
    GET_SET(bool, IsAttack);

    bool GetIsFixedDir() { return m_isFixedDir; }

    GET_SET(Vec3, InitialPos);
    GET_SET(Vec3, OriginPos);
    GET_SET(Vec3, CurPos);
    GET_SET(Vec3, OriginRot);
    GET_SET(Vec3, CurRot);

    GET_SET(GameObject*, TargetObject);
    GET_SET(ENEMY_TYPE, EnemyType);
    GET_SET(Ptr<APrefab>, FlowerProjectile);


    // ─── 벽 접촉 조회 ───
    bool GetIsBlockedLeft()  const { return m_WallContactLeft > 0; }
    bool GetIsBlockedRight() const { return m_WallContactRight > 0; }

    // 이벤트 구독 / 해제
    void SubscribeOnTakeDamage(EnemyDamageEvent _Event) { m_OnTakeDamageEvent = _Event; }
    void UnsubscribeOnTakeDamage()                      { m_OnTakeDamageEvent = nullptr; }

    //============
    // 생성, 소멸자
    //============
    CEnemyData();
    virtual ~CEnemyData();
};
