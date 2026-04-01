#include "pch.h"
#include "CEnemyEyes.h"
#include "GameObject.h"

CEnemyEyes::CEnemyEyes()
	: CScript(SCRIPT_TYPE::ENEMYEYES)
{
}

CEnemyEyes::~CEnemyEyes()
{
}

void CEnemyEyes::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 3)
	{
		// 디버그 들어오는것 확인
		int a = 0;
	}
}

void CEnemyEyes::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyEyes::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CEnemyEyes::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyEyes::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyEyes::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyEyes::EndOverlap);
}

void CEnemyEyes::Tick()
{
}

void CEnemyEyes::SaveToLevelFile(FILE* _File)
{
}

void CEnemyEyes::LoadFromLevelFile(FILE* _File)
{
}
