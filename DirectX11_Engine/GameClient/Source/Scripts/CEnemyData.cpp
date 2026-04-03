#include "pch.h"
#include "CEnemyData.h"

#include "LevelMgr.h"
#include "TimeMgr.h"

#include "CCollider2D.h"

#include "Source\Content\EnemyDamageState.h"
#include "CEnemyStateManager.h"
#include "CPlayerData.h"
#include "CPlayerMeleeTrigger.h"

CEnemyData::CEnemyData()
	: CScript(SCRIPT_TYPE::ENEMYDATA)
	// 멤버들은 추후 파일로 저장하여, 불러오는 방식을 사용합니다.
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_Speed(100.f)
	, m_JumpVelocity(300.f)
	, m_VelocityY(0.f)
	, m_Offset(0.f)
	, m_TimeSinceSpawn(0.f)
	, m_TimeInState(0.f)

	, m_Direction(1)		// 이동방향 1로 초기화

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)

	, m_OriginPos{}
	, m_CurPos{}

	// EnemyType은 기본으로 END입니다.
	// ImGui에서 지정해야 하며, 하지 않을경우 크래시
	, m_EnemyType(ENEMY_TYPE::END)
{
}

CEnemyData::~CEnemyData()
{
}

void CEnemyData::Init()
{
	// Level 시작 시, 벡터를 초기화
	ClearScriptParam();

	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FullHP, L"FullHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurHP, L"CurHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, L"Damage", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpVelocity, L"JumpVelocity", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_VelocityY, L"VelocityY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Offset, L"Offset", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_TimeSinceSpawn, L"TimeSinceSpawn", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_TimeInState, L"TimeInState", true, 0.f);
	
	AddScriptParam(SCRIPT_PARAM::INT, &m_Direction, L"Direction", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

	// enum class ENEMY_TYPE을 ImGui에서 편집할 수 있도록 전달
	// Level Play 전에 m_EnemyType을 미리 받을 수 있게 보장합니다.
	AddScriptParam(SCRIPT_PARAM::ENUM_CLASS, &m_EnemyType, L"EnemyType", true, 0.f,
		vector<wstring>{ L"DEMON", L"SKULL", L"FLYING", L"FLOWER", L"BOSS", L"END" });
}

void CEnemyData::Begin()
{
	// NOTE(26-03-31):
	// 문자열로 오브젝트를 지정하는 방식은 
	// Enemy가 게임에 많이 스폰되는 경우
	// 문자열이 같다고 보장 할 수 없습니다. 다른 방식 찾아보기
	// 
	// 조건문으로 타입을 정하는것이 아닌, 인스펙터에서
	// 타입을 지정하고, 이를 저장 및 불러오는 방식으로 변경하기
	m_TargetObject = GetOwner();
	m_EyeObject = GetOwner()->GetChild(ENEMY_EYES).Get();
	m_EyeObject->SetLayerIdx(9);

	// assert는 조건이 false일 때만 실행(중단)됩니다.
	assert(m_EnemyType != ENEMY_TYPE::END && "EnemyType is End");

	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(0.f, 0.f, 0.f);
	m_CurPos = m_OriginPos;

	// 스케일 x축값을 읽어와 음수인지 양수인지 판단하여 이동방향을 미리 결정합니다.
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	if (vScale.x > 0)
		m_Direction = 1;
	else
		m_Direction = -1;

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyData::EndOverlap);

	// m_TargetObject에 같이 있는 CEnemyStateManager 객체의 주소를 얻어와
	// SetUp 함수 호출, 호출 순서를 보장하기 위함
	// Script의 Begin을 호출한다면, 등록된 순서대로 각 콘텐츠 스크립트의 Begin이 호출되기 때문에
	// State 생성 부분을 m_EnemyType이 결정된 이후 호출하게 합니다.
	m_TargetObject->GetScript<CEnemyStateManager>()->SetUp();
}

void CEnemyData::TakeDamage(float _Damage)
{
	// 해당 함수를 Player쪽 공격 스크립트에서 호출하고 있기 때문에
	// HIT 상태 변경을 이쪽에서 처리합니다.
	
	// 죽은 상태에서 함수가 또 호출되면, HIT 상태로 되돌리기 X
	if (m_IsDead)
		return;

	// 실제 데미지 처리는 CEnemyData에서 진행
	// FSM 설계상 여기에서 진행하면 안됨
	float hp = GetCurHP();
	hp -= _Damage;
	SetCurHP(hp);

	ChangeState(ENEMY_STATE::HIT);

	if (hp <= 0.f)
	{
		// Dead 상태 호출
		m_IsDead = true;
		ChangeState(ENEMY_STATE::DEAD);
	}
}

void CEnemyData::ChangeState(ENEMY_STATE _State)
{
	//=========================================
	// CEnemyData 클래스에서만 사용할 상태 변경 함수
	//=========================================

	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();

	// map에서 해당 ENEMY_STATE 키가 존재하는지 확인하고 포인터를 반환
	EnemyState* pState = pMgr->GetStatusByCommonState(_State);
	if (pState == nullptr)
	{
		assert(false && "ChangeState: Requested ENEMY_STATE not found in map");
		return;
	}

	pMgr->SetCurStatus(pState);
	pMgr->ChangeState();
}

void CEnemyData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_IsDead == true)
	{
		// NOTE(26-04-02):
		// IsDead 리턴 처리를 해야, 겹쳐있을 때 enemy가 Dead 상태가 되어도
		// 다시 부활하는 것 같은 현상 방지할 수 있음, 단 짧은 시간내에
		// Overlap 판정으로 인한 Attack 상태 활성화로, Player가 데미지를 입는 현상이
		// 발견되었으므로, 필요하다면 데미지 처리를 막는 로직 작성
		return;
	}

	m_IsFalling = false;


	// Player와 충돌 시
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		ChangeState(ENEMY_STATE::ATTACK);
	}
}

void CEnemyData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	//if (m_IsDead == true)
	//	return;

	m_IsFalling = true;

	// HIT, DEAD 상태에서는 IDLE로 강제 전환하지 않음
	// 해당 상태들은 Flipbook 재생 완료 후 자체적으로 전이합니다.
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	ENEMY_STATE curState = pMgr->GetCurCommonState();

	if (curState == ENEMY_STATE::HIT || curState == ENEMY_STATE::DEAD)
		return;

	ChangeState(ENEMY_STATE::IDLE);
}

void CEnemyData::Tick()
{
	if (m_IsDead == true)
		return;

	// 소환 후 흐른 시간 계산
	m_TimeSinceSpawn += DT;
}

void CEnemyData::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
}

void CEnemyData::LoadFromLevelFile(FILE* _File)
{
	fread(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
}
