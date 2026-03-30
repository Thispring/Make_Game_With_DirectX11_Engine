#include "pch.h"
#include "CEnemyStateManager.h"
#include "GameObject.h"
#include "TimeMgr.h"

#include "LevelMgr.h"

#include "Source\Content\EnemyIdleState.h"
#include "Source\Content\EnemyMoveState.h"
#include "Source\Content\EnemyJumpState.h"
#include "Source\Content\EnemyAttackState.h"


CEnemyStateManager::CEnemyStateManager()
	: CScript(SCRIPT_TYPE::ENEMYSTATEMANAGER)
	, m_CurStatus(nullptr)
	, m_PrevStatus(nullptr)
	, m_vecStatus{}
{
}

// 복사 생성자: m_vecStatus 내부 객체들을 Clone()으로 복제
CEnemyStateManager::CEnemyStateManager(const CEnemyStateManager& _Origin)
    : CScript(SCRIPT_TYPE::ENEMYSTATEMANAGER)
    , m_EnemyData(_Origin.m_EnemyData) // Ptr 타입이 복사 가능하다고 가정
    , m_CurStatus(nullptr)
    , m_PrevStatus(nullptr)
{
    m_vecStatus.reserve(_Origin.m_vecStatus.size());

    // 각 상태 객체를 Clone하여 벡터에 추가
    for (const auto& srcPtr : _Origin.m_vecStatus)
    {
        if (srcPtr)
        {
            m_vecStatus.push_back(srcPtr->Clone());
        }
        else
        {
            m_vecStatus.push_back(nullptr);
        }
    }

    // m_CurStatus가 원본 벡터의 어느 요소인지 찾아서, 복제된 벡터의 동일 인덱스 요소의 포인터로 설정
    if (_Origin.m_CurStatus)
    {
        for (size_t i = 0; i < _Origin.m_vecStatus.size(); ++i)
        {
            if (_Origin.m_vecStatus[i].get() == _Origin.m_CurStatus)
            {
                m_CurStatus = (i < m_vecStatus.size() && m_vecStatus[i]) ? m_vecStatus[i].get() : nullptr;
                break;
            }
        }
    }
}

CEnemyStateManager::~CEnemyStateManager()
{
}


void CEnemyStateManager::Begin()
{
    m_EnemyData = GetOwner()->GetScript<CEnemyData>();

    // 상태 클래스 등록
    m_vecStatus.push_back(make_unique<EnemyIdleState>());  // 0
    m_vecStatus.push_back(make_unique<EnemyMoveState>());  // 1
    m_vecStatus.push_back(make_unique<EnemyJumpState>());  // 2
    m_vecStatus.push_back(make_unique<EnemyAttackState>());  // 3

    // 현재 상태를 Idle로 등록
    m_CurStatus = m_vecStatus[(int)PLAYER_STATE::IDLE].get();
    // 이전 상태 등록
    m_PrevStatus = m_CurStatus;
    m_CurStatus->Begin();
    // StateChange를 최초로 호출할때 true를 보장, flipbook 재생을 위함
    //SetStateChange();
    // 상태 번호는 Flipbook Index enum class를 전달받기
    //m_StateNum = (int)m_CurStatus->GetFlipbookIndex();
}

void CEnemyStateManager::Tick()
{
    // 필요에 따라 Tick에서 m_Status의 함수를 실행합니다.
    // 이전의 상태가 다르지 않을때만 Tick 수행
    if (m_PrevStatus == m_CurStatus)
    {
        m_CurStatus->Tick();
    }
}

void CEnemyStateManager::SaveToLevelFile(FILE* _File)
{
}

void CEnemyStateManager::LoadFromLevelFile(FILE* _File)
{
}
