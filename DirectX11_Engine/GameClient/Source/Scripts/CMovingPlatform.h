#pragma once
#include "CScript.h"

// ImGui로 멤버 변수를 설정하면
// 해당 수치만큼 플랫폼을 움직이는 스크립트 입니다.
class CMovingPlatform :
    public CScript
{

private:
    Vec3    m_OriginPos;
    Vec3    m_TargetPos;

    Vec3    m_OriginRot;
    Vec3    m_TargetRot;

    float   m_Time;
    float   m_Speed;
    float   m_Amplitude;
    float   m_RotAngle;

    int     m_Dir;          // 진행방향

    // Bob 멤버는 저장 X, 런타임에 저장된 위 멤버를 불러옴
    float   m_BobTime;              // 시간
    float   m_BobSpeed;             // 속도
    float   m_BobAmplitude;         // 진폭
    float   m_BobRotAngle;          // 회전각도

    // 플랫폼 위 탑승자에게 이동량을 전달하기 위한 delta
    Vec3    m_PrevPos;
    Vec3    m_Delta;

public:
    //=========
    // 멤버 함수
    //=========
    // 시간, 속도, 진폭, 회전각도 등을 매개변수로 받아 움직이기
    void PlatformMove();
    void PlatformRotation();

    Vec3 GetDelta() const { return m_Delta; }

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CMovingPlatform);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    //============
    // 생성, 소멸자
    //============
    CMovingPlatform();
    virtual ~CMovingPlatform();
};
