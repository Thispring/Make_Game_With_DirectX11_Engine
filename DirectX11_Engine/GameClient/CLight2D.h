#pragma once
#include "Component.h"

class CLight2D :
    public Component
{

private:
    Light2DInfo     m_Info;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    CLONE(CLight2D);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    const Light2DInfo& GetInfo() { return m_Info; }
    
    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    void SetLightType(LIGHT_TYPE _Type) { m_Info.Type = _Type; }

    Vec3 GetLightColor() { return m_Info.Color; }
    void SetLightColor(Vec3 _Color) { m_Info.Color = _Color; }

    Vec3 GetLightDir() { return m_Info.LightDir; }
    void SetLightDir(Vec3 _Dir) { m_Info.LightDir = _Dir; }

    Vec3 GetAmbient() { return m_Info.Ambient; }
    void SetAmbient(Vec3 _Amb) { m_Info.Ambient = _Amb; }

    float GetRadius() { return m_Info.Radius; }
    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }

    float GetAngle() { return m_Info.Angle; }
    void SetAngle(float _Anlge) { m_Info.Angle = _Anlge; }


    //============
    // 생성, 소멸자
    //============
    CLight2D();
    // 기본 복사 생성자 사용
    virtual ~CLight2D();
};
