#include "pch.h"
#include "EnemyDamageState.h"
#include "Source\Scripts\CEnemyStateManager.h"

#pragma region EnemyDamageState
EnemyDamageState::EnemyDamageState(Ptr<CEnemyData> _Data)
    : EnemyState(_Data)
{
}

EnemyDamageState::~EnemyDamageState()
{
}
#pragma endregion


#pragma region EnemyHitState
EnemyHitState::EnemyHitState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyHitState::~EnemyHitState()
{
}


void EnemyHitState::OnBegin()
{

}

void EnemyHitState::OnTick()
{
	// HIT Flipbook 재생이 끝나면 IDLE로 복귀
	if (m_EnemyData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
	{
		Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetTargetObject()->GetScript<CEnemyStateManager>();
		pMgr->SetCurStatus(pMgr->GetStatusByCommonState(ENEMY_STATE::IDLE));
		pMgr->ChangeState();
	}
}

void EnemyHitState::OnFinalTick()
{
    //Ptr<CEnemyStateManager> pMgr = m_EnemyData->GetOwner()->GetScript<CEnemyStateManager>();
    //pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)GetEnemyStateToParam(m_EnemyData->GetEnemyType(), ENEMY_STATE::IDLE)));
    //pMgr->ChangeState();
}


void EnemyHitState::SaveToLevelFile(FILE* _File)
{
}

void EnemyHitState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyHitState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion


#pragma region EnemyDeadState
EnemyDeadState::EnemyDeadState(Ptr<CEnemyData> _Data)
    : EnemyDamageState(_Data)
{

}

EnemyDeadState::~EnemyDeadState()
{
}

void EnemyDeadState::OnBegin()
{
}

void EnemyDeadState::OnTick()
{
    // Dead Flipbook 재생이 끝나면 비활성화
    if (m_EnemyData->GetTargetObject()->FlipbookRender()->GetFinish() == true)
    {
        //m_EnemyData->GetTargetObject()->GetChild(ENEMY_EYES)->SetIsActive(false);
        m_EnemyData->GetTargetObject()->SetIsActive(false);
    }
}

void EnemyDeadState::OnFinalTick()
{
}


void EnemyDeadState::SaveToLevelFile(FILE* _File)
{
}

void EnemyDeadState::LoadFromLevelFile(FILE* _File)
{
}

unique_ptr<EnemyState> EnemyDeadState::Clone() const
{
    return unique_ptr<EnemyState>();
}

#pragma endregion