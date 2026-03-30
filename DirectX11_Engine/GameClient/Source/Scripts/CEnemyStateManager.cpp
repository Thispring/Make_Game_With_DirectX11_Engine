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
// NOTE(26-03-30):
// 해당 복사 생성자가 Level이 Play 될때만 호출되는게 보장되는지 확인하고
// 호출이 보장이 되지 않는다면, EnemyState의 생성자에서 초기화 하는 부분을
// 다른 함수로 등록하거나, 다른 설계방식을 도입하기 
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


void CEnemyStateManager::SetUp()
{
    m_EnemyData = GetOwner()->GetScript<CEnemyData>();

    // 상태 클래스 등록
    // 각 상태 생성자에 소유자 이름(예: 현재 오브젝트 이름)을 전달
    // 람다식 or map에 미리 등록하여, 문자열에 맞는 Key 값 정수를 Begin 시점에
    // 조건문으로 전달하여, Enemy 특징에 맞는 객체 생성
    // 함수 포인터를 전달받아 상태 전이 고려

    // m_EnemyType 이 결정되었으므로, 필요한 상태 클래스를 조건분기하여, 각 상태 벡터에 생성 및 등록합니다.
    const wstring ownerName = GetOwner()->GetName();

    switch (m_EnemyData->GetEnemyType())
    {
    case ENEMY_TYPE::DEMON:
    {
        m_vecStatus.push_back(make_unique<EnemyIdleState>(ownerName));  // 0
        m_vecStatus.push_back(make_unique<EnemyMoveState>(ownerName));  // 1
        m_vecStatus.push_back(make_unique<EnemyJumpState>(ownerName));  // 2
        m_vecStatus.push_back(make_unique<EnemyAttackState>(ownerName));  // 3
    }
        break;
    case ENEMY_TYPE::SKULL:
    {
        m_vecStatus.push_back(make_unique<EnemyIdleState>(ownerName));  // 0
        m_vecStatus.push_back(make_unique<EnemyMoveState>(ownerName));  // 1
        m_vecStatus.push_back(make_unique<EnemyJumpState>(ownerName));  // 2
        m_vecStatus.push_back(make_unique<EnemyAttackState>(ownerName));  // 3
    }
        break;
    case ENEMY_TYPE::FLYING:

        break;
    case ENEMY_TYPE::FLOWER:

        break;
    case ENEMY_TYPE::BOSS:

        break;
    case ENEMY_TYPE::END:

        break;
    default:
        break;
    }

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

void CEnemyStateManager::Begin()
{
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
