#include "pch.h"
#include "CEnemyData.h"
#include "CEnemyStateManager.h"
#include "CCollider2D.h"
#include "LevelMgr.h"

CEnemyData::CEnemyData()
	: CScript(SCRIPT_TYPE::ENEMYDATA)
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_JumpVelocity(10.f)
	, m_Speed(100.f)

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)

	, m_OriginPos{}
	, m_CurPos{}

	, m_VelocityY(0.f)
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

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

}

void CEnemyData::Begin()
{
	m_TargetObject = LevelMgr::GetInst()->FindObjectByName(GetOwner()->GetName());

	// GetName 조건에 따라 ENEMY_TYPE 초기화를 다르게 진행
	// ENEMY_TYPE을 EnemyData에서 저장하고, TYPE 조건문 마다
	// 각자 다른 ENEMY_STATE를 호출받게 설정?
	if (GetOwner()->GetName() == L"mon1")
		m_EnemyType = ENEMY_TYPE::DEMON;
	else if (GetOwner()->GetName() == L"mon2")
		m_EnemyType = ENEMY_TYPE::SKULL;
	else if (GetOwner()->GetName() == L"mon3")
		m_EnemyType = ENEMY_TYPE::FLYING;
	else if (GetOwner()->GetName() == L"mon4")
		m_EnemyType = ENEMY_TYPE::FLOWER;
	else
		assert(nullptr);

	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(0.f, 0.f, 0.f);
	m_CurPos = m_OriginPos;

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyData::EndOverlap);

	// m_TargetObject에 같이 있는 CEnemyStateManager 객체의 주소를 얻어와
	// SetUp 함수 호출, 호출 순서를 보장하기 위함
	// Script의 Begin을 호출한다면, 등록된 순서대로 각 콘텐츠 스크립트의 Begin이 호출되기 때문에
	// State 생성 부분을 m_EnemyType이 결정된 이후 호출하게 합니다.
	m_TargetObject->GetScript<CEnemyStateManager>()->SetUp();
}

void CEnemyData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;

	// Layer Index 5번은 Player 투사체
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 5)
	{
		// Hit 상태로 변경하고, 데미지 계산
	}
}

void CEnemyData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = true;
}

void CEnemyData::Tick()
{

}

void CEnemyData::SaveToLevelFile(FILE* _File)
{

}

void CEnemyData::LoadFromLevelFile(FILE* _File)
{

}
