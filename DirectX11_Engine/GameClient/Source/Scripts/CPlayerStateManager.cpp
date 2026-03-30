#include "pch.h"
#include "CPlayerStateManager.h"

#include "CPlayerAnimator.h"

#include "LevelMgr.h"

#include "Source\Content\PlayerIdleState.h"
#include "Source\Content\PlayerMoveState.h"
#include "Source\Content\PlayerJumpState.h"
#include "Source\Content\PlayerMeleeAttackState.h"
#include "Source\Content\PlayerRangedAttackState.h"

int CPlayerStateManager::m_ChangeCount = 0;

bool CPlayerStateManager::IsStateChange()
{
    bool IsTemp = m_IsStateChange;
    m_IsStateChange = false;

    return IsTemp;
}

CPlayerStateManager::CPlayerStateManager()
	: CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
	, m_CurStatus(nullptr)
    , m_PrevStatus(nullptr)
	, m_vecStatus {}
    , m_StateNum(0)
{

}

// 복사 생성자: m_vecStatus 내부 객체들을 Clone()으로 복제
CPlayerStateManager::CPlayerStateManager(const CPlayerStateManager& _Origin)
    : CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
    , m_PlayerData(_Origin.m_PlayerData) // Ptr 타입이 복사 가능하다고 가정
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

CPlayerStateManager::~CPlayerStateManager()
{
}

void CPlayerStateManager::ChangeState()
{
    ++m_ChangeCount;


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
        SetStateChange();
        // m_ChangeCount는 최종적으로 0으로 유지
        m_ChangeCount = 0;

        m_StateNum = (int)m_CurStatus->GetFlipbookIndex();


        // PlayerAnimator를 불러와 Play 함수 호출
        GetOwner()->GetScript<CPlayerAnimator>()->Play();
    }

}

void CPlayerStateManager::Init()
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StateNum, L"StateNume", true, 0.f);
}

void CPlayerStateManager::Begin()
{
	// Player의 공유 데이터 클래스 등록
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();

	// 상태 클래스 등록
	m_vecStatus.push_back(make_unique<PlayerIdleState>());  // 0

	m_vecStatus.push_back(make_unique<PlayerMoveState>());  // 1 -> MoveState의 파생 클래스 WalkState로 변경해야함
	m_vecStatus.push_back(make_unique<PlayerJumpState>());  // 2

	m_vecStatus.push_back(make_unique<PlayerPunchState>());  // 3

	m_vecStatus.push_back(make_unique<PlayerHighKickState>());  // 4
	m_vecStatus.push_back(make_unique<PlayerMiddleKickState>());  // 5
	m_vecStatus.push_back(make_unique<PlayerLowKickState>());  // 6
	
    m_vecStatus.push_back(make_unique<PlayerEnergyBlastShotState>());  // 7


	// 현재 상태를 Idle로 등록
	m_CurStatus = m_vecStatus[(int)PLAYER_STATE::IDLE].get();
    // 이전 상태 등록
    m_PrevStatus = m_CurStatus;
	m_CurStatus->Begin();
    // StateChange를 최초로 호출할때 true를 보장, flipbook 재생을 위함
    SetStateChange();
    // 상태 번호는 Flipbook Index enum class를 전달받기
    m_StateNum = (int)m_CurStatus->GetFlipbookIndex();
}

void CPlayerStateManager::Tick()
{
	// 필요에 따라 Tick에서 m_Status의 함수를 실행합니다.
    // 이전의 상태가 다르지 않을때만 Tick 수행
	if (m_PrevStatus == m_CurStatus)
	{
		m_CurStatus->Tick();
	}
}

void CPlayerStateManager::SaveToLevelFile(FILE* _File)
{
}

void CPlayerStateManager::LoadFromLevelFile(FILE* _File)
{
}
