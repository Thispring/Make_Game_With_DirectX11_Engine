#include "pch.h"
#include "PlayerIdleStatus.h"


PlayerIdleStatus::PlayerIdleStatus()
	: m_FlipbookIndex(0)
{
}

PlayerIdleStatus::~PlayerIdleStatus()
{
}

void PlayerIdleStatus::SaveToLevelFile(FILE* _File)
{
}

void PlayerIdleStatus::LoadFromLevelFile(FILE* _File)
{
}

void PlayerIdleStatus::Begin()
{
	// Flipbook 재생

}

void PlayerIdleStatus::Tick()
{
}

void PlayerIdleStatus::FinalTick()
{
}

int PlayerIdleStatus::GetFlipbookIndex()
{
	return m_FlipbookIndex;
}
