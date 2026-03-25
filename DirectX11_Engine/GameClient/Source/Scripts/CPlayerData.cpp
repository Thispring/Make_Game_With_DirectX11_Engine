#include "pch.h"
#include "CPlayerData.h"
#include "LevelMgr.h"

CPlayerData::CPlayerData()
	: CScript(SCRIPT_TYPE::PLAYERDATA)
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_Speed(120.f)

	, m_DeathCount(0)

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)

	, m_TargetObject(nullptr)
{
}

CPlayerData::~CPlayerData()
{
}

void CPlayerData::Init()
{
	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FullHP, L"FullHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurHP, L"CurHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, L"Damage", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::INT, &m_DeathCount, L"DeathCount", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);
	//AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetObject, L"TargetObjectName", true, 0.f);
}

void CPlayerData::Begin()
{
	m_TargetObject = LevelMgr::GetInst()->FindObjectByName(L"Player");

	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerData::EndOverlap);
}

void CPlayerData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;
}

void CPlayerData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = true;
}

void CPlayerData::Tick()
{

}

void CPlayerData::SaveToLevelFile(FILE* _File)
{
}

void CPlayerData::LoadFromLevelFile(FILE* _File)
{
}
