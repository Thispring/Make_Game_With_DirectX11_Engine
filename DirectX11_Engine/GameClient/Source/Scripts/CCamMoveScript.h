#pragma once
#include "CScript.h"
#include "GameObject.h"


// 카메라 모드를 설정하여, 이동방식을 다르게 설계
enum class CAM_MOVE_MODE
{
    DEBUG,  // 게임 플레이 중 디버그용, 프리카메라 방식으로 이동 
    INGAME  // 플레이어의 이동을 따라다니는 카메라 (횡스크롤 방식)
};

class CCamMoveScript :
    public CScript
{

private:
    CAM_MOVE_MODE   m_MoveMode;

    Ptr<GameObject> m_Target;         // Pos를 추적할 타겟 오브젝트 (Player)

    Vec3            m_DebugOriPos;    // Debug 모드 초기 위치
    Vec3            m_DebugOriRot;    // Debug 모드 초기 회전

    Vec3            m_InGamePos;      // InGame 모드 위치
    Vec3            m_InGameRot;      // InGame 모드 회전

    bool    m_isMoving;     // 이동중인지 여부

public:
    //=========
    // 멤버 함수
    //=========
    void MoveOrigin();
    void MovingOrigin();
    void OrthoCamMove();
    void PrespecCamMove();
    void MouseCamMove();

    // Vec3의 각 요소의 절댓값을 계산하는 함수
    Vec3 Vec3Abs(const Vec3& v) 
    {
        return {
            fabsf(v.x), // float의 절댓값은 fabsf() 사용
            fabsf(v.y),
            fabsf(v.z)
        };
    }


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void Begin() override;
    // Script를 상속 받았다면 Tick을 필수로 선언해야 함
    virtual void Tick() override;   
    CLONE(CCamMoveScript);
    
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CCamMoveScript();
    virtual ~CCamMoveScript();
};
