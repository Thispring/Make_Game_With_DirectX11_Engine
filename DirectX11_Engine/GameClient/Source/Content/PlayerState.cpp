#include "pch.h"
#include "PlayerState.h"

#include "LevelMgr.h"
#include "TimeMgr.h"

PlayerState::PlayerState(Ptr<CPlayerData> _Data)
{
	// 문자열로 찾는게 아닌, StateManager에서 생성할때
	// StateManager의 멤버로 CPlayerData 객체 주소를 전달
	m_PlayerData = _Data;
}

PlayerState::~PlayerState()
{

}

void PlayerState::Init()
{
}

void PlayerState::ApplyGravity()
{
	if (m_PlayerData->GetIsFalling() == true)
	{
		// 1. 중력 가속도를 현재 속도에 누적 (v = v + a * dt)
		// 아래로 떨어지는 것이므로 속도값을 증가(또는 감소)시킵니다.
		float curVelocity = m_PlayerData->GetVelocityY();
		curVelocity -= GRAVITY_CONSTANT * DT;
		m_PlayerData->SetVelocityY(curVelocity);


		// 2. 누적된 속도를 위치에 반영 (p = p + v * dt)
		Vec3 vPos = m_PlayerData->GetTargetObject()->Transform()->GetRelativePos();
		vPos.y += DT * curVelocity;

		m_PlayerData->GetTargetObject()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		// 땅에 닿아있다면 속도를 0으로 초기화해둬야 다음 추락 시 정상 작동합니다.
		m_PlayerData->SetVelocityY(0.f);
	}
}
