#include "pch.h"
#include "CPlayerData.h"
#include "LevelMgr.h"
#include "AssetMgr.h"

CPlayerData::CPlayerData()
	: CScript(SCRIPT_TYPE::PLAYERDATA)
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_JumpVelocity(500.f)
	, m_Speed(250.f)

	, m_DeathCount(0)
	, m_DirNum(1)

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)
	, m_IsJumping(false)

	, m_OriginPos {}
	, m_CurPos {}

	, m_TargetObject(nullptr)
	, m_AnchorObject(nullptr)
	, m_EnergyBlast(nullptr)

	, m_VelocityY(0.f)
{
}

CPlayerData::~CPlayerData()
{
}

void CPlayerData::Init()
{
	// Level 시작 시, 벡터를 초기화
	ClearScriptParam();

	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FullHP, L"FullHP", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurHP, L"CurHP", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, L"Damage", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpVelocity, L"JumpVelocity", false, 0.f);

	AddScriptParam(SCRIPT_PARAM::INT, &m_DeathCount, L"DeathCount", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsJumping, L"IsJumping", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_EnergyBlast, L"EnergyBlast", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PTR, &m_TargetObject, L"TargetObject", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PTR, &m_AnchorObject, L"AnchorObject", true, 0.f);
}

void CPlayerData::Begin()
{
	// 문자열 방식으로 찾는 방법은 최대한 줄이기
	m_TargetObject = GetOwner();
	m_AnchorObject = GetOwner()->GetChild(PLAYER_PROJECTILE_ANCHOR);

	// NOTE(26-03-32):
	// 자식 오브젝트 1, 2는 근거리 용, Collider를 가지고 있는 자식 오브젝트입니다.
	// 항상 Layer 4번을 보장받을 수 있도록 Begin에서 4번으로 최종 세팅
	GetOwner()->GetChild(PLAYER_KICK_ANCHOR)->SetLayerIdx(4);
	GetOwner()->GetChild(PLAYER_PUNCH_ANCHOR)->SetLayerIdx(4);
	
	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(0.f, 0.f, 0.f);
	m_CurPos = m_OriginPos;

	// Scale을 받아와서 초기 방향 정보 초기화
	Vec3 vScale = m_TargetObject->Transform()->GetRelativeScale();
	if (vScale.x < 0)
		m_DirNum = -1;
	else
		m_DirNum = 1;

	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerData::EndOverlap);
}

void CPlayerData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 2)
	{
		// Layer 2번과 충돌했을 때만 m_IsFalling 상태를 변경
		m_IsFalling = false;
	}
}

void CPlayerData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CPlayerData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 2)
	{
		// Layer 2번과 충돌했을 때만 m_IsFalling 상태를 변경
		m_IsFalling = true;
	}
}

void CPlayerData::Tick()
{

}

void CPlayerData::SaveToLevelFile(FILE* _File)
{
	// 파일로 저장해야할 멤버 정리
	// m_TargetObject, m_AnchorObject는 Begin에서 초기화 하므로 파일 저장 X

	// 파일 저장 조건을
	// 프로그램을 재실행하는가? 라는 조건을 두고
	// 먼저 재실행한다 하더라도, 저장 및 불러오기가 필요한 멤버 먼저 저장

	// m_OriginPos, m_CurPos
	// m_FullHP, m_CurHP, m_Damage, m_Speed, m_JumpVelocity
	// m_DeathCount
	//
	// 위 멤버는 재실행할때, 해당 정보를 불러와서 이어하길 원한다면 파일에 저장합니다.
	// 
	//fwrite(&m_FullHP, sizeof(float), 1, _File);
	//fwrite(&m_Damage, sizeof(float), 1, _File);
	//fwrite(&m_Speed, sizeof(float), 1, _File);
	//fwrite(&m_JumpVelocity, sizeof(float), 1, _File);

	SaveAssetRef(_File, m_EnergyBlast.Get());
}

void CPlayerData::LoadFromLevelFile(FILE* _File)
{
	m_EnergyBlast = LoadAssetRef<APrefab>(_File);
}

bool CPlayerData::GetIsAttack()
{
	bool isAttack = m_IsAttack;
	m_IsAttack = false;

	return isAttack;
}

void CPlayerData::SetIsAttack()
{
	m_IsAttack = true;
}

void CPlayerData::OffIsAttack()
{
	// false로 강제 전환 함수
	m_IsAttack = false;
}
