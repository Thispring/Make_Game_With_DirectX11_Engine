#include "pch.h"
#include "PlayerDeathState.h"
#include "Source\Scripts\CPlayerStateManager.h"

#include "SoundMgr.h"

PlayerDeathState::PlayerDeathState(Ptr<CPlayerData> _Data)
	: PlayerState(_Data)
{
	m_FlipbookIndex = PLAYER_STATE::DEATH;
}

PlayerDeathState::~PlayerDeathState()
{

}

void PlayerDeathState::Begin()
{
	SoundMgr::GetInst()->PlaySFX(L"PlayerDeath", 1, false);
	// GET_SET 매크로에서 Set함수에 +1 매개변수를 전달하는 것은 단순히 정수 1을 의미, 아래와 같이 작성해야함
	m_PlayerData->SetDeathCount(m_PlayerData->GetDeathCount() + 1);
}

void PlayerDeathState::Tick()
{
	// Flipbook 재생이 끝나면 Respawn 호출
	if (m_PlayerData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
	{
		Ptr<CPlayerStateManager> pMgr = m_PlayerData->GetTargetObject()->GetScript<CPlayerStateManager>();
		pMgr->Respawn();
	}
}

void PlayerDeathState::FinalTick()
{
}

PLAYER_STATE PlayerDeathState::GetFlipbookIndex()
{
	return PLAYER_STATE();
}

void PlayerDeathState::SaveToLevelFile(FILE* _File)
{
}

void PlayerDeathState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<PlayerState> PlayerDeathState::Clone() const
{
	return unique_ptr<PlayerState>();
}
