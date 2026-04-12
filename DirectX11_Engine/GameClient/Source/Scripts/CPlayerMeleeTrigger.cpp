#include "pch.h"
#include "CPlayerMeleeTrigger.h"
#include "GameObject.h"
#include "CEnemyData.h"


CPlayerMeleeTrigger::CPlayerMeleeTrigger()
	: CScript(SCRIPT_TYPE::PLAYERMELEETRIGGER)
	, m_PlayerData(nullptr)
{
}

CPlayerMeleeTrigger::~CPlayerMeleeTrigger()
{
}


void CPlayerMeleeTrigger::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::ENEMY)
	{
		// 여기에서 _OtherCollider한테 데미지값을 전달해야함
		float dmg = m_PlayerData->GetDamage();

		// Kick 공격일 때 데미지 증가
		if (_OwnCollider->GetOwner()->GetName() == L"Kick_Anchor")
			dmg += 2.f;

		_OtherCollider->GetOwner()->GetScript<CEnemyData>()->TakeDamage(dmg);
	}
}

void CPlayerMeleeTrigger::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerMeleeTrigger::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerMeleeTrigger::Init()
{
}

void CPlayerMeleeTrigger::Begin()
{
	m_PlayerData = GetOwner()->GetParent()->GetScript<CPlayerData>();
	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerMeleeTrigger::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerMeleeTrigger::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerMeleeTrigger::EndOverlap);

	// Begin 초기화가 끝나고 스스로 비활성화 하게 변경
	GetOwner()->Collider2D()->SetEnabled(false);
}

void CPlayerMeleeTrigger::Tick()
{
}

void CPlayerMeleeTrigger::SaveToLevelFile(FILE* _File)
{
}

void CPlayerMeleeTrigger::LoadFromLevelFile(FILE* _File)
{
}
