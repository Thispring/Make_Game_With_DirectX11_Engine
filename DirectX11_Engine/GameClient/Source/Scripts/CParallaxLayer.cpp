#include "pch.h"
#include "CParallaxLayer.h"
#include "LevelMgr.h"
#include "CTransform.h"
#include "TimeMgr.h"

CParallaxLayer::CParallaxLayer()
    : CScript(SCRIPT_TYPE::PARALLAXLAYER)
    , m_TargetPlayer(nullptr)
    , m_ParallaxFactor(0.5f)
    , m_Smooth(8.0f)
{
}

CParallaxLayer::~CParallaxLayer()
{
}

void CParallaxLayer::Begin()
{
    // Player 오브젝트는 레벨에서 "Player" 이름으로 찾음 (프로젝트에서 동일 사용 중)
    m_TargetPlayer = LevelMgr::GetInst()->FindObjectByName(L"Player");
    if (m_TargetPlayer != nullptr)
        m_PrevTargetPos = m_TargetPlayer->Transform()->GetRelativePos();
}

void CParallaxLayer::Tick()
{
    if (m_TargetPlayer == nullptr)
        return;

    Vec3 curTargetPos = m_TargetPlayer->Transform()->GetRelativePos();
    Vec3 delta = curTargetPos - m_PrevTargetPos;

    // 목표 이동량 (즉시 적용)
    Vec3 desiredPos = Transform()->GetRelativePos() + delta * m_ParallaxFactor;

    if (m_Smooth <= 0.f)
    {
        Transform()->SetRelativePos(desiredPos);
    }
    else
    {
        // 간단한 LERP 기반 부드럽기: t = 1 - exp(-k * DT) 형태의 근사 사용 가능
        float t = 1.0f - expf(-m_Smooth * DT);
        Vec3 cur = Transform()->GetRelativePos();
        Vec3 lerped = cur + (desiredPos - cur) * t;
        Transform()->SetRelativePos(lerped);
    }

    m_PrevTargetPos = curTargetPos;
}

void CParallaxLayer::SaveToLevelFile(FILE* _File)
{
}

void CParallaxLayer::LoadFromLevelFile(FILE* _File)
{
}
