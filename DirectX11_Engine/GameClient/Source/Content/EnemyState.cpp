#include "pch.h"
#include "EnemyState.h"

#include "LevelMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

EnemyState::EnemyState(Ptr<CEnemyData> _Data)
	: m_FlipbookIndex()
{
	m_EnemyData = _Data;
}

EnemyState::~EnemyState()
{
}

// Test 용 코드
static bool isTest = false;
void EnemyState::ApplyGravity()
{
	// 임시 비활성화 Key 코드 추가하기
	if (KEY_TAP(KEY::ALPHA8))
	{
		// bool Test 변수 토글
		isTest = !isTest;
		return;
	}

	if (isTest)
		return;

	if (m_EnemyData->GetIsFalling() == true)
	{
		// 1. 중력 가속도를 현재 속도에 누적 (v = v + a * dt)
		// 아래로 떨어지는 것이므로 속도값을 증가(또는 감소)시킵니다.
		float curVelocity = m_EnemyData->GetVelocityY();
		curVelocity -= GRAVITY_CONSTANT * DT;
		m_EnemyData->SetVelocityY(curVelocity);


		// 2. 누적된 속도를 위치에 반영 (p = p + v * dt)
		Vec3 vPos = m_EnemyData->GetTargetObject()->Transform()->GetRelativePos();
		vPos.y += DT * curVelocity;

		m_EnemyData->GetTargetObject()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		// 땅에 닿아있다면 속도를 0으로 초기화해둬야 다음 추락 시 정상 작동합니다.
		m_EnemyData->SetVelocityY(0.f);
	}
}

void EnemyState::CalTimeInState()
{
	float time = m_EnemyData->GetTimeInState();
	time += DT;
	m_EnemyData->SetTimeInState(time);
}

void EnemyState::ClearTimeInState()
{
	m_EnemyData->SetTimeInState(0.f);
}

// 템플릿 메서드 구현: 공통 로직 실행 후 자식 훅 호출
void EnemyState::Begin()
{
	ClearTimeInState();
	OnBegin();
}

void EnemyState::Tick()
{
	ApplyGravity();
	CalTimeInState();
	OnTick();
}

void EnemyState::FinalTick()
{
	OnFinalTick();
}
