#pragma once
#include "CScript.h"

class CEditorCamMoveScript :
    public CScript
{

private:
    Vec3    m_OriginPos;    // 초기 위치
    Vec3    m_OriginRot;    // 초기 회전
    Vec3    m_CurPos;       // 현재 위치
    Vec3    m_CurRot;       // 현재 회전

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
    virtual void Begin() override;
    // Script를 상속 받았다면 Tick을 필수로 선언해야 함
    virtual void Tick() override;
    CLONE(CEditorCamMoveScript);


    //============
    // 생성, 소멸자
    //============
    CEditorCamMoveScript();
    virtual ~CEditorCamMoveScript();
};
