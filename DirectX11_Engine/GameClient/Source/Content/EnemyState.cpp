#include "pch.h"
#include "EnemyState.h"
#include "LevelMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

EnemyState::EnemyState()
{
	if (m_EnemyData == nullptr)
	{
		// NOTE(26-03-30):
		// CEnemyData 주소를 받아오는 부분이 mon1에 의존되어 있음
		// Enemy 종류에 따라 각자 다른 오브젝트의 EnemyData 주소를 받아올 수 있게
		// 설계 방식을 변경해야 합니다.
		//
		// 방법1. 생성자 파라미터로 이름을 받고, 해당 이름을 통해 오브젝트 찾기
		//Ptr<GameObject> pObj = LevelMgr::GetInst()->FindObjectByName(L"mon1");
		//m_EnemyData = pObj->GetScript<CEnemyData>();
	}
}


// 생성자에서 소유자 이름을 받아 해당 이름의 객체를 찾아 EnemyData를 설정
EnemyState::EnemyState(const std::wstring& ownerName)
	: m_OwnerName(ownerName)
{
	// NOTE(26-03-30):
	// FindObjectByName 함수는 현재 Level에 등록된 오브젝트의 문자열을 검색하므로
	// Level에 등록되지 않은 Prefab의 경우 해당 함수가 작동하지 않습니다.
	// Prefab으로 등록을 한다고 하면, Enemy GameObject, Prefab을 관리하는
	// 관리자 클래스를 등록하여, 해당 클래스에 멤버로 Prefab, GameObject를 등록하고
	// enum class로 부모 객체 GameObject를 반환하는 함수를 따로 만들어
	// 생성 시점에 호출합니다.
	Ptr<GameObject> pObj = LevelMgr::GetInst()->FindObjectByName(m_OwnerName);
	if (pObj != nullptr)
	{
		m_EnemyData = pObj->GetScript<CEnemyData>();
	}
}


EnemyState::~EnemyState()
{

}


// Test 용 코드
static bool isTest = false;
void EnemyState::ApplyGravity()
{
	//// 임시 비활성화 Key 코드 추가하기
	//if (KEY_TAP(KEY::G))
	//{
	//	// bool Test 변수 토글
	//	isTest = !isTest;
	//	return;
	//}

	//if (isTest)
	//	return;

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
