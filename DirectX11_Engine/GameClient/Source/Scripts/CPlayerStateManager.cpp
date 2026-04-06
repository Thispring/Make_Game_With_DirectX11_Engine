#include "pch.h"
#include "CPlayerStateManager.h"

#include "CPlayerAnimator.h"
#include "CPlayerController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "RandomMgr.h"

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
    , m_bPendingRevive(false)
    , m_fReviveDelay(-1.f)
	//, m_bInputLocked(false)
{

}

// 복사 생성자: m_vecStatus 내부 객체들을 Clone()으로 복제
// stl 스마트 포인터를 사용중인 멤버가 있어서 따로 구현
CPlayerStateManager::CPlayerStateManager(const CPlayerStateManager& _Origin)
    : CScript(SCRIPT_TYPE::PLAYERSTATEMANAGER)
    , m_PlayerData(_Origin.m_PlayerData) // Ptr 타입이 복사 가능하다고 가정
    , m_CurStatus(nullptr)
    , m_PrevStatus(nullptr)
    , m_bPendingRevive(_Origin.m_bPendingRevive)
    , m_fReviveDelay(_Origin.m_fReviveDelay)
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

    // 진입 상태에 따라 입력 잠금 자동 설정
    // — 잠금 상태: Controller 전체 입력 차단
    // — 해제 상태(Idle, Walk 등): Animator가 ChangeState(Idle) 호출 시 자동 해제
    PLAYER_STATE newState = m_CurStatus->GetFlipbookIndex();
    //m_bInputLocked = (newState == PLAYER_STATE::PUNCH          ||
    //                  newState == PLAYER_STATE::MIDDLE_KICK    ||
    //                  newState == PLAYER_STATE::HIGH_KICK      ||
    //                  newState == PLAYER_STATE::LOW_KICK       ||
    //                  newState == PLAYER_STATE::JUMP           ||
    //                  newState == PLAYER_STATE::ENERGYBLAST_SHOT);
}

void CPlayerStateManager::TakeDamage(float _Damage)
{
    if (m_PlayerData->GetIsDead() == true)
        return;

    float curHP = m_PlayerData->GetCurHP();
    curHP -= _Damage;

    if (curHP <= 0)
    {
        m_PlayerData->SetIsDead(true);

        //// 사망 시 키 발견 상태 초기화
        //Ptr<CPlayerController> pController = GetOwner()->GetScript<CPlayerController>();
        //pController->SetHasDied(true);
        //pController->ResetKeyRevealed();
        return;
    }

    m_PlayerData->SetCurHP(curHP);
}

void CPlayerStateManager::Respawn()
{
    //======================================
    // 초기 스탯으로 초기화 + Origin 위치로 이동
    //======================================
    
    // 체력
    m_PlayerData->SetCurHP(m_PlayerData->GetFullHP());

    // 위치
    GetOwner()->Transform()->SetRelativePos(m_PlayerData->GetOriginPos());


    //================
    // 공격 키 난수 로직
    //================
    Ptr<CPlayerController> pController = m_PlayerData->GetTargetObject()->GetScript<CPlayerController>();
    KEY arryKey[3] = {};

    RandomMgr::GetInst()->ShuffleKeyNum();

    for (UINT i = 0; i < 3; ++i)
    {
        arryKey[i] = RandomMgr::GetInst()->GetRandomKey(3);
    }

    pController->SetPunchKey(arryKey[PUNCH_KEY]);
    pController->SetKickKey(arryKey[KICK_KEY]);
    pController->SetBlastShotKey(arryKey[BLAST_SHOT_KEY]);

    assert(arryKey[0] != arryKey[1] && "Respawn: PunchKey == KickKey");
    assert(arryKey[1] != arryKey[2] && "Respawn: KickKey == BlastShotKey");
    assert(arryKey[0] != arryKey[2] && "Respawn: PunchKey == BlastShotKey");

    // 키 발견 초기화
    pController->ResetKeyRevealed();

    //============================================================
    // IsDead 해제를 즉시 처리하지 않고 Tick으로 위임
    // 0.f → 다음 프레임, N.f → N초 후
    //============================================================
    m_bPendingRevive = true;
    m_fReviveDelay   = 0.f;
}

void CPlayerStateManager::Init()
{

}

void CPlayerStateManager::Begin()
{
	// Player의 공유 데이터 클래스 등록
	m_PlayerData = GetOwner()->GetScript<CPlayerData>();
	// 상태 클래스 등록 (map 기반 — FLIPBOOK::PLAYER 인덱스 쌍으로 저장)
	m_mapStatus[PLAYER_STATE::IDLE]             = std::make_pair(make_unique<PlayerIdleState>(m_PlayerData),            FLIPBOOK::PLAYER::IDLE);
	m_mapStatus[PLAYER_STATE::WALK]             = std::make_pair(make_unique<PlayerMoveState>(m_PlayerData),            FLIPBOOK::PLAYER::WALK);
	m_mapStatus[PLAYER_STATE::JUMP]             = std::make_pair(make_unique<PlayerJumpState>(m_PlayerData),            FLIPBOOK::PLAYER::JUMP);
	m_mapStatus[PLAYER_STATE::PUNCH]            = std::make_pair(make_unique<PlayerPunchState>(m_PlayerData),           FLIPBOOK::PLAYER::PUNCH);
	m_mapStatus[PLAYER_STATE::HIGH_KICK]        = std::make_pair(make_unique<PlayerHighKickState>(m_PlayerData),        FLIPBOOK::PLAYER::HIGH_KICK);
	m_mapStatus[PLAYER_STATE::MIDDLE_KICK]      = std::make_pair(make_unique<PlayerMiddleKickState>(m_PlayerData),      FLIPBOOK::PLAYER::MIDDLE_KICK);
	m_mapStatus[PLAYER_STATE::LOW_KICK]         = std::make_pair(make_unique<PlayerLowKickState>(m_PlayerData),         FLIPBOOK::PLAYER::LOW_KICK);
	m_mapStatus[PLAYER_STATE::ENERGYBLAST_SHOT] = std::make_pair(make_unique<PlayerEnergyBlastShotState>(m_PlayerData), FLIPBOOK::PLAYER::ENERGYBLAST_SHOT);

	// 현재 상태를 Idle로 등록 (map에서 안전하게 조회)
	auto it = m_mapStatus.find(PLAYER_STATE::IDLE);
	if (it != m_mapStatus.end())
		m_CurStatus = it->second.first.get();
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
    //================================================
    // 지연 부활 처리 — Tick 최상단에서 수행
    // Respawn()이 같은 프레임에서 호출되더라도
    // 이 블록은 이미 지나쳤으므로 다음 프레임부터 카운트
    //================================================
    if (m_bPendingRevive)
    {
        m_fReviveDelay -= DT;
        if (m_fReviveDelay <= 0.f)
        {
            m_bPendingRevive = false;
            m_fReviveDelay   = -1.f;
            m_PlayerData->SetIsDead(false);
        }
    }

    // Player Respawn Test
    if (KEY_PRESSED(KEY::ALPHA1))
    {
        m_PlayerData->SetIsDead(true);
        Respawn();
    }

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
