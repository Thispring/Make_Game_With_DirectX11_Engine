#include "pch.h"
#include "CParallaxLayer.h"
#include "LevelMgr.h"
#include "CTransform.h"
#include "TimeMgr.h"

#include "CPlayerData.h"


CParallaxLayer::CParallaxLayer()
    : CScript(SCRIPT_TYPE::PARALLAXLAYER)
    , m_TargetPlayer(nullptr)
    , m_ParallaxFactor(0.5f)
    , m_Smooth(8.0f)
    , m_ScrollOffset(0.f, 0.f, 0.f) 
{
}

CParallaxLayer::~CParallaxLayer()
{
    for (size_t i = 0; i < m_firstBackGround.size(); ++i)
        m_firstBackGround[i]->Transform()->SetRelativePos(m_firstBackGroundInitPos[i]);
    for (size_t i = 0; i < m_secondBackGround.size(); ++i)
        m_secondBackGround[i]->Transform()->SetRelativePos(m_secondBackGroundInitPos[i]);
}

void CParallaxLayer::Begin()
{
    // Player 오브젝트는 레벨에서 "Player" 이름으로 찾음 (프로젝트에서 동일 사용 중)
    m_TargetPlayer = LevelMgr::GetInst()->FindObjectByName(L"Player");
    if (m_TargetPlayer != nullptr)
    {
        m_CurTargetPos = m_PrevTargetPos = m_TargetPlayer->Transform()->GetRelativePos();
    }

    vector<Ptr<GameObject>> pChild = GetOwner()->GetChild();

    for (size_t i = 0; i < pChild.size(); ++i)
    {
        if (i < 4)
            m_firstBackGround.push_back(pChild[i]);
        else
            m_secondBackGround.push_back(pChild[i]);
    }

    for (auto& obj : m_firstBackGround)
        m_firstBackGroundInitPos.push_back(obj->Transform()->GetRelativePos());
    for (auto& obj : m_secondBackGround)
        m_secondBackGroundInitPos.push_back(obj->Transform()->GetRelativePos());
}

void CParallaxLayer::Tick()
{
    if (m_TargetPlayer == nullptr)
        return;

    Ptr<CPlayerData> pData = m_TargetPlayer->GetScript<CPlayerData>();
    float speed = pData->GetSpeed();

    m_CurTargetPos = m_TargetPlayer->Transform()->GetRelativePos();

    const float EPSILON = 1e-5f;
    Vec3 delta = m_CurTargetPos - m_PrevTargetPos;
    if (delta.Length() < EPSILON)
        return;

    // 방향 벡터 정규화
    Vec3 dir = delta;
    dir.Normalize();

    // 이동량 계산 및 누적
    Vec3 move = dir * speed * DT;
    m_ScrollOffset += move;

    // 배경 위치에 스크롤 오프셋 적용
    Transform()->SetRelativePos(m_ScrollOffset);

    m_PrevTargetPos = m_CurTargetPos;
}

void CParallaxLayer::SaveToLevelFile(FILE* _File)
{
}

void CParallaxLayer::LoadFromLevelFile(FILE* _File)
{
}
