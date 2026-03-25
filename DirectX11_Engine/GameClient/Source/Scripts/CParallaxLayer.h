#pragma once
#include "CScript.h"
#include "GameObject.h"
#include "SimpleMath.h"

// 배경 움직임을 담당하는 스크립트 입니다.
class CParallaxLayer :
    public CScript
{
private: 
    // 배경은 Sky, Ground, Sea, Cloud 4개 * 2 = 8개로 구성되어있습니다.
    // 최상위 부모를 먼저 움직여보고, 부자연스럽다면 개별 자식 오브젝트를 개별로 컨트롤

    Ptr<GameObject>     m_TargetPlayer;   // Player 오브젝트 참조
    vector<Ptr<GameObject>> m_firstBackGround;
    vector<Ptr<GameObject>> m_secondBackGround;

    vector<Vec3>        m_firstBackGroundInitPos;
    vector<Vec3>        m_secondBackGroundInitPos;

    Vec3                m_PrevTargetPos;  // 이전 프레임의 플레이어 위치
    Vec3                m_CurTargetPos;
    Vec3                m_ScrollOffset; // 누적 이동량(스크롤 오프셋)

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
