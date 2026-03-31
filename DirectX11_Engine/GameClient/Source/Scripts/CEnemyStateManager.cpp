#include "pch.h"
#include "CEnemyStateManager.h"

#include "CEnemyAnimator.h"

#include "GameObject.h"
#include "TimeMgr.h"

#include "LevelMgr.h"

#include "Source\Content\EnemyIdleState.h"
#include "Source\Content\EnemyMoveState.h"
#include "Source\Content\EnemyJumpState.h"
#include "Source\Content\EnemyAttackState.h"
#include "Source\Content\EnemyDamageState.h"


CEnemyStateManager::CEnemyStateManager()
	: CScript(SCRIPT_TYPE::ENEMYSTATEMANAGER)
	, m_CurStatus(nullptr)
	, m_PrevStatus(nullptr)
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
    // 원본 map의 각 상태를 Clone하여 새로운 map에 복제
    for (const auto& pair : _Origin.m_mapStatus)
    {
        if (pair.second)
        {
            m_mapStatus[pair.first] = pair.second->Clone();
        }
    }

    // m_CurStatus가 원본 map의 어느 요소인지 찾아서, 복제된 map의 동일 키 요소의 포인터로 설정
    if (_Origin.m_CurStatus)
    {
        for (const auto& pair : _Origin.m_mapStatus)
        {
            if (pair.second.get() == _Origin.m_CurStatus)
            {
                auto it = m_mapStatus.find(pair.first);
                if (it != m_mapStatus.end())
                {
                    m_CurStatus = it->second.get();
                }
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
    // 조건분기 하는 이유:
    // 종류에 따라 필요한 상태가 다르기 때문입니다. 예를들어 FLYING 타입은 Jump 기능이 없으므로
    // JumpState 클래스를 생성할 필요가 없습니다.

    // 인덱스 순서는 Flipbook 순서에만 영향이 있습니다.
    // 실제 로직에는 영향이 없어야 하는게 맞습니다.
    // StateManager에서는 현재 상태를 가리키는 포인터에
    // Tick 함수를 호출시키기 때문에, 인덱스 문제는 없어야함
    // clear any existing registrations
    m_mapStatus.clear();

    switch (m_EnemyData->GetEnemyType())
    {
    case ENEMY_TYPE::DEMON:
    {
        m_mapStatus[ENEMY_COMMON_STATE::IDLE] = make_unique<EnemyIdleState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::MOVE] = make_unique<EnemyMoveState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::JUMP] = make_unique<EnemyJumpState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::ATTACK] = make_unique<EnemyAttackState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::HIT] = make_unique<EnemyHitState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::DEAD] = make_unique<EnemyDeadState>(m_EnemyData);
    }
        break;
    case ENEMY_TYPE::SKULL:
    {
        m_mapStatus[ENEMY_COMMON_STATE::IDLE] = make_unique<EnemyIdleState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::MOVE] = make_unique<EnemyMoveState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::JUMP] = make_unique<EnemyJumpState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::ATTACK] = make_unique<EnemyAttackState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::HIT] = make_unique<EnemyHitState>(m_EnemyData);
        m_mapStatus[ENEMY_COMMON_STATE::DEAD] = make_unique<EnemyDeadState>(m_EnemyData);
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

    // 현재 상태를 Idle로 등록 (map에서 안전하게 조회)
    auto it = m_mapStatus.find(ENEMY_COMMON_STATE::IDLE);
    if (it != m_mapStatus.end())
    {
        m_CurStatus = it->second.get();
    }
    else
    {
        m_CurStatus = nullptr;
    }
    // 이전 상태 등록
    m_PrevStatus = m_CurStatus;
    if (m_CurStatus)
        m_CurStatus->Begin();

    // idle flipbook 재생 강제 보장
    GetOwner()->FlipbookRender()->Play(-1, 10, -1);
    //GetOwner()->GetScript<CEnemyAnimator>()->Play();
}

bool CEnemyStateManager::IsChange()
{
    bool IsTemp = m_IsChange;
    m_IsChange = false;

    return IsTemp;
}

void CEnemyStateManager::ChangeState()
{
    // 상태를 변경할때 마다 호출
    // 이전 상태 클래스의 FinalTick을 호출하고
    // 바뀐 상태의 Begin을 호출
    m_PrevStatus->FinalTick();
    m_CurStatus->Begin();

    // 이전 상태와 현재 상태가 같이 않았다면
    // 바뀐 상태를 갱신    
    if (m_PrevStatus != m_CurStatus)
    {
        m_PrevStatus = m_CurStatus;

        // 성공적으로 바뀌었다면 true 반환
        SetChange();

        // Animator를 불러와 Play 함수 호출
        GetOwner()->GetScript<CEnemyAnimator>()->Play();
    }
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
