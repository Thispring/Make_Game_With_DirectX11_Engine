#include "pch.h"
#include "CPlayerStateManager.h"

#include "CPlayerAnimator.h"

#include "LevelMgr.h"

#include "Source\Content\PlayerIdleState.h"
#include "Source\Content\PlayerMoveState.h"
#include "Source\Content\PlayerJumpState.h"
#include "Source\Content\PlayerMeleeAttackState.h"
#include "Source\Content\PlayerRangedAttackState.h"

bool CPlayerStateManager::IsChange()
{
    bool IsTemp = m_IsChange;
    m_IsChange = false;

    return IsTemp;
}

CPlayerStateManager::CPlayerStateManager()
	: CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
	, m_CurStatus(nullptr)
    , m_PrevStatus(nullptr)

{

}

// 복사 생성자: m_vecStatus 내부 객체들을 Clone()으로 복제
// stl 스마트 포인터를 사용중인 멤버가 있어서 따로 구현
CPlayerStateManager::CPlayerStateManager(const CPlayerStateManager& _Origin)
    : CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
    , m_PlayerData(_Origin.m_PlayerData) // Ptr 타입이 복사 가능하다고 가정
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

CPlayerStateManager::~CPlayerStateManager()
{
}

void CPlayerStateManager::ChangeState()
{
    // ChangeState함수는, Key 입력을 받는 Controller 내지는
    // 체력 이벤트를 받는 다른 클래스에서 해당 함수를 호출 시킵니다.


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

        // PlayerAnimator를 불러와 Play 함수 호출
        GetOwner()->GetScript<CPlayerAnimator>()->Play();
    }
}

void CPlayerStateManager::Init()
{

}

void CPlayerStateManager::Begin()
{
	// Player의 공유 데이터 클래스 등록
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();
    // 상태 클래스 등록 (map 기반)
    m_mapStatus[PLAYER_STATE::IDLE] = make_unique<PlayerIdleState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::WALK] = make_unique<PlayerMoveState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::JUMP] = make_unique<PlayerJumpState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::PUNCH] = make_unique<PlayerPunchState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::HIGH_KICK] = make_unique<PlayerHighKickState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::MIDDLE_KICK] = make_unique<PlayerMiddleKickState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::LOW_KICK] = make_unique<PlayerLowKickState>(m_PlayerData);
    m_mapStatus[PLAYER_STATE::ENERGYBLAST_SHOT] = make_unique<PlayerEnergyBlastShotState>(m_PlayerData);

    // 현재 상태를 Idle로 등록 (map에서 안전하게 조회)
    auto it = m_mapStatus.find(PLAYER_STATE::IDLE);
    if (it != m_mapStatus.end())
        m_CurStatus = it->second.get();
    else
        m_CurStatus = nullptr;
    // 이전 상태 등록
    m_PrevStatus = m_CurStatus;
	m_CurStatus->Begin();
    // StateChange를 최초로 호출할때 true를 보장, flipbook 재생을 위함
    SetChange();

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
