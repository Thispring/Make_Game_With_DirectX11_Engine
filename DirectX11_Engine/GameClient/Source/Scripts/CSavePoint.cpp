#include "pch.h"
#include "CSavePoint.h"

#include "GameObject.h"

#include "GameMgr.h"

CSavePoint::CSavePoint()
	: CScript(SCRIPT_TYPE::SAVEPOINT)
{
}

CSavePoint::~CSavePoint()
{
}

void CSavePoint::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// Player와 충돌했을 때, GameMgr을 통해 SavePoint 이벤트를 호출시켜
		// Player의 세이브 위치 정보를 갱신합니다.
		GameMgr::GetInst()->RegisterPlayerSave(GetOwner()->Transform()->GetRelativePos());


		// 사라지는 Flipbook 재생
		// 재생이 끝나면 기능이 다했으므로 비활성화 or 삭제 요청을 합니다.
		GetOwner()->FlipbookRender()->Play(1, 30, 0);
	}
}

void CSavePoint::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CSavePoint::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CSavePoint::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CSavePoint::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CSavePoint::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CSavePoint::EndOverlap);
}

void CSavePoint::Tick()
{

}

void CSavePoint::SaveToLevelFile(FILE* _File)
{
}

void CSavePoint::LoadFromLevelFile(FILE* _File)
{
}
