#include "pch.h"
#include "CEnemyData.h"
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
	m_TargetObject = LevelMgr::GetInst()->FindObjectByName(L"mon1");

	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(0.f, 0.f, 0.f);
	m_CurPos = m_OriginPos;

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyData::EndOverlap);
}

void CEnemyData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;
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
