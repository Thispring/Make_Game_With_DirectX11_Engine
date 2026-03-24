#pragma once
#include "CScript.h"
#include "GameObject.h"
#include "SimpleMath.h"

// 배경 움직임을 담당하는 스크립트 입니다.
class CParallaxLayer :
    public CScript
{
private:
    Ptr<GameObject>     m_TargetPlayer;   // Player 오브젝트 참조
    // NOTE(26-03-24): 빌드 시, Vec3 링킹 오류 발생
    // Math 헤더를 참조할 수 없어서 발생한 것으로 추정
    Vec3                m_PrevTargetPos;  // 이전 프레임의 플레이어 위치
    float               m_ParallaxFactor; // 0.0 ~ 1.0 (작을수록 더 멀리 있는 효과)
    float               m_Smooth;         // 0 = 즉시, 클수록 부드럽게 따라감

public:
    //=========
    // 멤버 함수
    //=========


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;
    CLONE(CParallaxLayer);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    void SetParallaxFactor(float f) { m_ParallaxFactor = f; }
    float GetParallaxFactor() const { return m_ParallaxFactor; }
    void SetSmooth(float s) { m_Smooth = s; }
    float GetSmooth() const { return m_Smooth; }


    //============
    // 생성, 소멸자
    //============
    CParallaxLayer();
    virtual ~CParallaxLayer();
};
