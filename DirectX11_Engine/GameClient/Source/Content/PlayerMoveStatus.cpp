#include "pch.h"
#include "PlayerMoveStatus.h"

PlayerMoveStatus::PlayerMoveStatus()
{
}

PlayerMoveStatus::~PlayerMoveStatus()
{
}

void PlayerMoveStatus::SaveToLevelFile(FILE* _File)
{
}

void PlayerMoveStatus::LoadFromLevelFile(FILE* _File)
{
}

void PlayerMoveStatus::Begin()
{
}

void PlayerMoveStatus::Tick()
{
	// Tick을 받으면 m_Target의 Pos 값을 받아와 변경합니다.
	// 상세 움직임(=오른쪽, 왼쪽, 위, 아래 등)을 enum으로 정의하여
	// Controller에서 Key 입력에 따라 상세 움직임을 변경합니다.
	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();
	Vec3 vRot = GetTargetObject()->Transform()->GetRelativeRot();


	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeRot(vRot);
}

void PlayerMoveStatus::FinalTick()
{
}
