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
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 6)
	{
		// 여기에서 _OtherCollider한테 데미지값을 전달해야함
		float dmg = m_PlayerData->GetDamage();
		_OtherCollider->GetOwner()->GetScript<CEnemyData>()->ApplyDamage(dmg);
	}
}

void CPlayerMeleeTrigger::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerMeleeTrigger::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerMeleeTrigger::Begin()
{
	m_PlayerData = GetOwner()->GetParent()->GetScript<CPlayerData>();

	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerMeleeTrigger::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerMeleeTrigger::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerMeleeTrigger::EndOverlap);
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
