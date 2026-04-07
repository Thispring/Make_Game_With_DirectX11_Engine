#include "pch.h"
#include "CPortal.h"

#include "LevelMgr.h"

CPortal::CPortal()
	: CScript(SCRIPT_TYPE::PORTAL)
{
}

CPortal::~CPortal()
{
}

void CPortal::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// Ending Level로 전환
		LevelMgr::GetInst()->ChangeEnding();
	}
}

void CPortal::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPortal::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPortal::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CPortal::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPortal::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPortal::EndOverlap);
}

void CPortal::Tick()
{
}

void CPortal::SaveToLevelFile(FILE* _File)
{
}

void CPortal::LoadFromLevelFile(FILE* _File)
{
}
