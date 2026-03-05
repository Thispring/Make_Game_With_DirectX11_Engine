#include "pch.h"

#include "CMissileScript.h"
#include "CPlayerScript.h"
#include "CTransform.h"

#include "TimeMgr.h"
#include "TaskMgr.h"
#include "LevelMgr.h"

#include "GameObject.h"

CMissileScript::CMissileScript()
	: CScript(SCRIPT_TYPE::MISSILESCRIPT)
	, m_travelTime(0.f)
	, m_TargetObj(nullptr)
	, p_TargetObj(nullptr)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CMissileScript::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	_OwnCollider->GetName();
	_OtherCollider->GetName();

	_OwnCollider->GetOwner()->GetName();
	_OtherCollider->GetOwner()->GetName();

	// 1초가 지난 후, 충돌된 오브젝트와 설정한 타겟 오브젝트의 이름이 같다면,
	// 오브젝트 모두 WeaponAnchor이어야지 해당 조건문에 접근 가능
	if (m_travelTime > 1.f && m_TargetObj->GetName() == _OtherCollider->GetOwner()->GetName())
	{
		// 미사일을 삭제 처리하고
		// 플레이어의 애니메이션 처리를 중단
		Destroy();
		
		m_PlayerObj->SetMissileReturn(true);
		m_PlayerObj->SetMissileCount(0);
	}
}

void CMissileScript::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CMissileScript::Begin()
{
	m_OriginPos = GetOwner()->Transform()->GetRelativePos();
	// 되돌아올 위치를 첫 위치로 설정
	SetReturnTarget(m_OriginPos);

	// Player의 자식 오브젝트 이름을 전달
	SetTargetObj((LevelMgr::GetInst()->FindObjectByName(L"WeaponAnchor")));

	// Player 스크립트 등록
	m_PlayerObj = m_TargetObj->GetParent()->GetScript<CPlayerScript>();

	//Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMissileScript::BeginOverlap);
	//Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&CMissileScript::Overlap);
	//Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMissileScript::EndOverlap);

	ADD_DYNAMIC_BEGIN_OVERLAP(CMissileScript::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CMissileScript::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CMissileScript::EndOverlap);

}

void CMissileScript::Tick()
{
	// 자신의 포지션 값을 m_Offset에 갱신
	m_Offset = GetOwner()->Transform()->GetRelativePos();

	// 멤버에 첫 생성 후 얼마만큼 시간이 흘렀는지 저장하는 변수 추가
	m_travelTime += DT;

	Chase();
}

// 목표를 향해 위치를 업데이트하는 함수
void CMissileScript::Chase() 
{
	float m_Speed = 1000.f;

	// 목표가 없거나, 생성 후 4초가 지나면 미사일 파괴
	if (nullptr == m_TargetObj)
	{
		Destroy();
		return;
	}

	// 1. 미사일의 현재 '월드' 위치를 가져옵니다.
	Vec3 vMissilePos = GetOwner()->Transform()->GetRelativePos();

	if (m_travelTime > 1.f)
	{
		// --- 추적 단계 ---
		// 2. 목표의 '월드' 위치를 가져옵니다.
		Vec3 vTargetPos = m_TargetObj->Transform()->GetWorldPos();

		// 3. 미사일에서 목표를 향하는 방향 벡터를 계산합니다. (목표 위치 - 나의 위치)
		Vec3 vDir = vTargetPos - vMissilePos;
		vDir.Normalize(); // 방향 벡터의 크기를 1로 만듭니다.

		// 4. 방향으로 (속도 * 시간) 만큼 이동시킵니다.
		vMissilePos += vDir * m_Speed * DT;
	}
	else
	{
		// --- 초기 발사 단계 ---
		// 미사일의 로컬 '위쪽' 방향을 가져옵니다.
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		vMissilePos += vUp * 500.f * DT;
	}

	// 5. 계산된 새로운 '월드' 위치를 적용합니다.
	//GetOwner()->Transform()->SetWorldPos(vMissilePos);
	GetOwner()->Transform()->SetRelativePos(vMissilePos); // Relative로 적용해야 가능
}
