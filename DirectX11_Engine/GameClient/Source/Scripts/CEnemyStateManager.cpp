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
#include "Source\Content\EnemyPatrolState.h"
#include "Source\Content\EnemyChaseState.h"


CEnemyStateManager::CEnemyStateManager()
	: CScript(SCRIPT_TYPE::ENEMYSTATEMANAGER)
    , m_EnemyData(nullptr)
	, m_CurStatus(nullptr)
	, m_PrevStatus(nullptr)
    , m_mapStatus{}
    , m_IsChange(false)
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
    , m_mapStatus{}
    , m_IsChange(false)
{
    // 원본 map의 각 상태를 Clone하여 새로운 map에 복제
    for (const auto& pair : _Origin.m_mapStatus)
    {
        if (pair.second.first)
        {
            m_mapStatus[pair.first] = std::make_pair(pair.second.first->Clone(), pair.second.second);
        }
    }

    // m_CurStatus가 원본 map의 어느 요소인지 찾아서, 복제된 map의 동일 키 요소의 포인터로 설정
    if (_Origin.m_CurStatus)
    {
        for (const auto& pair : _Origin.m_mapStatus)
        {
            if (pair.second.first.get() == _Origin.m_CurStatus)
            {
                auto it = m_mapStatus.find(pair.first);
                if (it != m_mapStatus.end())
                {
                    m_CurStatus = it->second.first.get();
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
        m_mapStatus[ENEMY_STATE::IDLE]   = std::make_pair(make_unique<EnemyIdleState>(m_EnemyData),   FLIPBOOK::DEMON::IDLE);
        m_mapStatus[ENEMY_STATE::MOVE]   = std::make_pair(make_unique<EnemyMoveState>(m_EnemyData),   FLIPBOOK::DEMON::MOVE);
        m_mapStatus[ENEMY_STATE::JUMP]   = std::make_pair(make_unique<EnemyJumpState>(m_EnemyData),   FLIPBOOK::DEMON::JUMP);
        m_mapStatus[ENEMY_STATE::ATTACK] = std::make_pair(make_unique<EnemyAttackState>(m_EnemyData), FLIPBOOK::DEMON::ATTACK);
        m_mapStatus[ENEMY_STATE::HIT]    = std::make_pair(make_unique<EnemyHitState>(m_EnemyData),    FLIPBOOK::DEMON::HIT);
        m_mapStatus[ENEMY_STATE::DEAD]   = std::make_pair(make_unique<EnemyDeadState>(m_EnemyData),   FLIPBOOK::DEMON::DEAD);
        m_mapStatus[ENEMY_STATE::PATROL] = std::make_pair(make_unique<EnemyPatrolState>(m_EnemyData), FLIPBOOK::DEMON::MOVE);
        m_mapStatus[ENEMY_STATE::CHASE]  = std::make_pair(make_unique<EnemyChaseState>(m_EnemyData),  FLIPBOOK::DEMON::MOVE);
    }
        break;
    case ENEMY_TYPE::SKULL:
    {
        m_mapStatus[ENEMY_STATE::IDLE]   = std::make_pair(make_unique<EnemyIdleState>(m_EnemyData),   FLIPBOOK::SKULL::IDLE);
        m_mapStatus[ENEMY_STATE::MOVE]   = std::make_pair(make_unique<EnemyMoveState>(m_EnemyData),   FLIPBOOK::SKULL::MOVE);
        m_mapStatus[ENEMY_STATE::ATTACK] = std::make_pair(make_unique<EnemyAttackState>(m_EnemyData), FLIPBOOK::SKULL::ATTACK);
        m_mapStatus[ENEMY_STATE::HIT]    = std::make_pair(make_unique<EnemyHitState>(m_EnemyData),    FLIPBOOK::SKULL::HIT);
        m_mapStatus[ENEMY_STATE::DEAD]   = std::make_pair(make_unique<EnemyDeadState>(m_EnemyData),   FLIPBOOK::SKULL::DEAD);
        m_mapStatus[ENEMY_STATE::PATROL] = std::make_pair(make_unique<EnemyPatrolState>(m_EnemyData), FLIPBOOK::SKULL::MOVE);
        m_mapStatus[ENEMY_STATE::CHASE]  = std::make_pair(make_unique<EnemyChaseState>(m_EnemyData),  FLIPBOOK::SKULL::MOVE);

        m_mapStatus[ENEMY_STATE::GHOST_SKULL]  = std::make_pair(make_unique<EnemyGhostSkullState>(m_EnemyData),  FLIPBOOK::SKULL::GHOST_SKULL);
        m_mapStatus[ENEMY_STATE::GHOST_SKULL_MOVE]  = std::make_pair(make_unique<EnemyGhostSkullMoveState>(m_EnemyData),  FLIPBOOK::SKULL::GHOST_SKULL_MOVE);
    }
        break;
    case ENEMY_TYPE::FLYING:
        // FLYING의 IDLE과 MOVE는 동일한 Flipbook 사용(IDLE로 인덱스 사용), JUMP 사용 X
        m_mapStatus[ENEMY_STATE::IDLE] = std::make_pair(make_unique<EnemyIdleState>(m_EnemyData), FLIPBOOK::FLYING::IDLE);
        m_mapStatus[ENEMY_STATE::MOVE] = std::make_pair(make_unique<EnemyMoveState>(m_EnemyData), FLIPBOOK::FLYING::IDLE);
        m_mapStatus[ENEMY_STATE::ATTACK] = std::make_pair(make_unique<EnemyAttackState>(m_EnemyData), FLIPBOOK::FLYING::ATTACK);
        m_mapStatus[ENEMY_STATE::HIT] = std::make_pair(make_unique<EnemyHitState>(m_EnemyData), FLIPBOOK::FLYING::HIT);
        m_mapStatus[ENEMY_STATE::DEAD] = std::make_pair(make_unique<EnemyDeadState>(m_EnemyData), FLIPBOOK::FLYING::DEAD);
        m_mapStatus[ENEMY_STATE::PATROL] = std::make_pair(make_unique<EnemyPatrolState>(m_EnemyData), FLIPBOOK::FLYING::IDLE);
        m_mapStatus[ENEMY_STATE::CHASE] = std::make_pair(make_unique<EnemyChaseState>(m_EnemyData), FLIPBOOK::FLYING::IDLE);
        break;
    case ENEMY_TYPE::FLOWER:
        // FLOWER는 MOVE 사용 X, JUMP는 필수사용 X
        m_mapStatus[ENEMY_STATE::IDLE] = std::make_pair(make_unique<EnemyIdleState>(m_EnemyData), FLIPBOOK::FLOWER::IDLE);
        m_mapStatus[ENEMY_STATE::JUMP] = std::make_pair(make_unique<EnemyMoveState>(m_EnemyData), FLIPBOOK::FLOWER::JUMP);
        m_mapStatus[ENEMY_STATE::ATTACK] = std::make_pair(make_unique<EnemyAttackState>(m_EnemyData), FLIPBOOK::FLOWER::MELEE_ATTACK);
        // FLOWER 타입은 EnemyEyes에 분기처리를 하여, Patrol 상태가 아닌 원거리 공격 상태로 전환
        m_mapStatus[ENEMY_STATE::RANGED_ATTACK] = std::make_pair(make_unique<EnemyRangedAttackState>(m_EnemyData), FLIPBOOK::FLOWER::RANGED_ATTACK);

        m_mapStatus[ENEMY_STATE::HIT] = std::make_pair(make_unique<EnemyHitState>(m_EnemyData), FLIPBOOK::FLOWER::HIT);
        m_mapStatus[ENEMY_STATE::DEAD] = std::make_pair(make_unique<EnemyDeadState>(m_EnemyData), FLIPBOOK::FLOWER::DEAD);
        m_mapStatus[ENEMY_STATE::PATROL] = std::make_pair(make_unique<EnemyPatrolState>(m_EnemyData), FLIPBOOK::FLOWER::IDLE);
        break;
    case ENEMY_TYPE::BOSS:
        //m_mapStatus[ENEMY_STATE::IDLE] = std::make_pair(make_unique<EnemyIdleState>(m_EnemyData), FLIPBOOK::BOSS::IDLE);
        //m_mapStatus[ENEMY_STATE::MOVE] = std::make_pair(make_unique<EnemyMoveState>(m_EnemyData), FLIPBOOK::BOSS::MOVE);
        break;
    }

    if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::TEST)
        return;

    // 현재 상태를 Idle로 등록 (map에서 안전하게 조회)
    auto it = m_mapStatus.find(ENEMY_STATE::IDLE);
    if (it != m_mapStatus.end())
    {
        m_CurStatus = it->second.first.get();
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
    // Flipbook Play 함수에 index로 -1을 받았을 때 처리해둠
    GetOwner()->FlipbookRender()->Play(-1, 10, -1);
}

bool CEnemyStateManager::IsChange()
{
    bool IsTemp = m_IsChange;
    m_IsChange = false;

    return IsTemp;
}

EnemyState* CEnemyStateManager::GetStatusByCommonState(ENEMY_STATE _State)
{
    auto it = m_mapStatus.find(_State);
    return (it != m_mapStatus.end()) ? it->second.first.get() : nullptr;
}

EnemyState* CEnemyStateManager::GetStatusByIndex(int _Idx)
{
    // ENEMY_STATE는 각 타입별로 0..5 값을 사용하므로 공통 상태로 캐스트 가능
    ENEMY_STATE common = static_cast<ENEMY_STATE>(_Idx);
    return GetStatusByCommonState(common);
}

ENEMY_STATE CEnemyStateManager::GetCurCommonState()
{
    // map에 저장된 정보로 현재 상태에 맞는 ENEMY_STATE 반환
    for (const auto& pair : m_mapStatus)
    {
        if (pair.second.first.get() == m_CurStatus)
            return pair.first;
    }

    assert(false && "Current state not found in m_mapStatus");
    return ENEMY_STATE::IDLE; // assert 이후 도달하지 않지만, 컴파일 경고 방지용
}

int CEnemyStateManager::GetFlipBookIndex()
{
    for (const auto& pair : m_mapStatus)
    {
        if (pair.second.first.get() == m_CurStatus)
            return pair.second.second;
    }

    assert(false && "Current state not found in m_mapStatus");
    return 0;
}

void CEnemyStateManager::ChangeState()
{
    // 이미 같은 상태라면 Begin() 재호출 방지
    //if (m_PrevStatus == m_CurStatus)
    //    return;

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
    if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::TEST)
        return;
}

void CEnemyStateManager::Tick()
{
    if (m_EnemyData->GetEnemyType() == ENEMY_TYPE::TEST)
        return;

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
