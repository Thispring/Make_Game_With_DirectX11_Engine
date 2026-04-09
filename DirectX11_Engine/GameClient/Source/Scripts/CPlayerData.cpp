#include "pch.h"
#include "CPlayerData.h"

#include "CPlayerStateManager.h"
#include "CMovingPlatform.h"

#include "LevelMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"

CPlayerData::CPlayerData()
	: CScript(SCRIPT_TYPE::PLAYERDATA)
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(3.5f)
	, m_JumpVelocity(500.f)
	, m_Speed(250.f)

	, m_DeathCount(0)
	, m_Direction(1)

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)
	, m_IsJumping(false)

	, m_OriginPos {}
	, m_CurPos {}

	, m_TargetObject(nullptr)
	, m_AnchorObject(nullptr)
	, m_EnergyBlast(nullptr)

	, m_VelocityY(0.f)
	, m_GroundNormal(0.f, 1.f, 0.f)
	, m_fCoyoteTimer(0.f)
	, m_GroundContactCount(0)

	, m_WallContactLeft(0)
	, m_WallContactRight(0)
{
}

CPlayerData::~CPlayerData()
{
}

void CPlayerData::Init()
{
	// Level 시작 시, 벡터를 초기화
	ClearScriptParam();

	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FullHP, L"FullHP", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurHP, L"CurHP", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, L"Damage", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", false, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpVelocity, L"JumpVelocity", false, 0.f);

	AddScriptParam(SCRIPT_PARAM::INT, &m_DeathCount, L"DeathCount", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsJumping, L"IsJumping", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_EnergyBlast, L"EnergyBlast", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PTR, &m_TargetObject, L"TargetObject", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PTR, &m_AnchorObject, L"AnchorObject", true, 0.f);
}

void CPlayerData::Begin()
{
	// 문자열 방식으로 찾는 방법은 최대한 줄이기
	m_TargetObject = GetOwner();
	m_AnchorObject = GetOwner()->GetChild(PLAYER_PROJECTILE_ANCHOR);
	// NOTE(26-04-02):
	// PLAYER_PROJECTILE_ANCHOR도 3번 Layer여서 Enemy 충돌판정에 사용
	// 중복을 막기 위해 인덱스번호 5번으로 고정
	m_AnchorObject->SetLayerIdx(5);

	// NOTE(26-03-32):
	// 자식 오브젝트 1, 2는 근거리 용, Collider를 가지고 있는 자식 오브젝트입니다.
	// 항상 Layer 4번을 보장받을 수 있도록 Begin에서 4번으로 최종 세팅
	GetOwner()->GetChild(PLAYER_KICK_ANCHOR)->SetLayerIdx(4);
	GetOwner()->GetChild(PLAYER_PUNCH_ANCHOR)->SetLayerIdx(4);
	
	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(-4600.f, 250.f, 100.f);	// NOTE(26-04-03): 고정 위치 등록
	m_CurPos = m_OriginPos;

	// OriginPos로 위치 초기화
	GetOwner()->Transform()->SetRelativePos(m_OriginPos);

	// Scale을 받아와서 초기 방향 정보 초기화
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	if (vScale.x < 0)
		m_Direction = -1;
	else
		m_Direction = 1;

	ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerData::EndOverlap);
}

void CPlayerData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// 추락 시, 사망 처리
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::OUT_OF_BOUNDS)
	{
		Ptr<CPlayerStateManager> pMgr = GetOwner()->GetScript<CPlayerStateManager>();

		SetIsDead(true);
		pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::DEATH));
		pMgr->ChangeState();
	}

	// ─── 바닥 / 천장 (Layer 16) ───
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		Matrix invSlope  = _OtherCollider->GetWorldMat().Invert();
		Vec3 localCenter = XMVector3TransformCoord(_OwnCollider->GetWorldCenter(), invSlope);

		if (localCenter.y >= 0.f)
		{
			++m_GroundContactCount;
			m_IsFalling = false;
			m_fCoyoteTimer = 0.f;

			Matrix slopeMat = _OtherCollider->GetWorldMat();
			Vec3 normal = Vec3(slopeMat._21, slopeMat._22, slopeMat._23);
			normal.Normalize();
			m_GroundNormal = (fabsf(normal.y) > 0.99f) ? Vec3(0.f, 1.f, 0.f) : normal;
		}
		else
		{
			if (m_VelocityY > 0.f)
				m_VelocityY = 0.f;
		}
	}

	// ─── 벽 (Layer 17) ───
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::WALL_COLLIDER)
	{
		// 월드 X 좌표로 좌/우 판별 — localCenter 부호 반전 문제 회피
		float ownX  = _OwnCollider->GetWorldCenter().x;
		float wallX = _OtherCollider->GetWorldCenter().x;

		if (ownX >= wallX)
			++m_WallContactLeft;    // 벽이 왼쪽 → 왼쪽 이동 차단
		else
			++m_WallContactRight;   // 벽이 오른쪽 → 오른쪽 이동 차단
	}
}

void CPlayerData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	int otherLayer = _OtherCollider->GetOwner()->GetLayerIdx();

	// ─── 바닥 / 천장 (Layer 16) ───
	if (otherLayer == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		Matrix ownMat  = _OwnCollider->GetWorldMat();
		Vec3 ownYAxis  = Vec3(ownMat._21, ownMat._22, ownMat._23);

		Matrix invSlope  = _OtherCollider->GetWorldMat().Invert();
		Vec3 centerWorld = _OwnCollider->GetWorldCenter();
		Vec3 localCenter = XMVector3TransformCoord(centerWorld, invSlope);

		if (localCenter.y >= 0.f)
		{
			// ─── 바닥 ───
			Vec3 footWorld = centerWorld;
			footWorld.x -= ownYAxis.x * 0.5f;
			footWorld.y -= ownYAxis.y * 0.5f;
			footWorld.z -= ownYAxis.z * 0.5f;

			Vec3 localFoot = XMVector3TransformCoord(footWorld, invSlope);
			float localPenetration = 0.5f - localFoot.y;

			Matrix slopeMat = _OtherCollider->GetWorldMat();
			Vec3 pos = GetOwner()->Transform()->GetRelativePos();

			// 1. penetration 보정 (양수일 때만)
			if (localPenetration > 0.f)
			{
				pos.x += localPenetration * slopeMat._21;
				pos.y += localPenetration * slopeMat._22;
			}

			// 2. Moving Platform delta (penetration과 무관하게 항상 적용)
			Ptr<CMovingPlatform> pPlatform = _OtherCollider->GetOwner()->GetScript<CMovingPlatform>();
			if (pPlatform != nullptr)
			{
				Vec3 delta = pPlatform->GetDelta();
				pos.x += delta.x;
			}

			// 3. 한 번만 호출
			GetOwner()->Transform()->SetRelativePos(pos);
		}
		else
		{
			// ─── 천장 ───
			Vec3 headWorld = centerWorld;
			headWorld.x   += ownYAxis.x * 0.5f;
			headWorld.y   += ownYAxis.y * 0.5f;
			headWorld.z   += ownYAxis.z * 0.5f;

			Vec3 localHead = XMVector3TransformCoord(headWorld, invSlope);
			float localPenetration = localHead.y + 0.5f;
			if (localPenetration <= 0.f)
				return;

			Matrix slopeMat = _OtherCollider->GetWorldMat();
			Vec3   pos      = GetOwner()->Transform()->GetRelativePos();
			pos.x -= localPenetration * slopeMat._21;
			pos.y -= localPenetration * slopeMat._22;
			GetOwner()->Transform()->SetRelativePos(pos);

			if (m_VelocityY > 0.f)
				m_VelocityY = 0.f;
		}
	}

	// ─── 벽 (Layer 17) ───
	if (otherLayer == (int)LEVEL_0_LAYER::WALL_COLLIDER)
	{
		// 월드 좌표 기반 보정 — localCenter 부호 반전으로 인한 관통 방지
		float ownCenterX  = _OwnCollider->GetWorldCenter().x;
		float wallCenterX = _OtherCollider->GetWorldCenter().x;

		// 벽 반폭 (월드 X축 스케일)
		Matrix wallMat = _OtherCollider->GetWorldMat();
		float wallHalfW = Vec3(wallMat._11, wallMat._12, wallMat._13).Length() * 0.5f;

		// 자신 반폭 (월드 X축 스케일)
		Matrix ownMat = _OwnCollider->GetWorldMat();
		float ownHalfW = Vec3(ownMat._11, ownMat._12, ownMat._13).Length() * 0.5f;

		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		if (ownCenterX >= wallCenterX)
		{
			// 플레이어가 벽 오른쪽: 왼쪽 모서리가 벽 오른쪽 면을 넘지 않도록
			float penetration = (wallCenterX + wallHalfW) - (ownCenterX - ownHalfW);
			if (penetration > 0.f)
				pos.x += penetration;
		}
		else
		{
			// 플레이어가 벽 왼쪽: 오른쪽 모서리가 벽 왼쪽 면을 넘지 않도록
			float penetration = (ownCenterX + ownHalfW) - (wallCenterX - wallHalfW);
			if (penetration > 0.f)
				pos.x -= penetration;
		}

		GetOwner()->Transform()->SetRelativePos(pos);
	}
}

void CPlayerData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// ─── 바닥 이탈 (Layer 16) ───
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		Matrix invSlope  = _OtherCollider->GetWorldMat().Invert();
		Vec3 localCenter = XMVector3TransformCoord(_OwnCollider->GetWorldCenter(), invSlope);

		if (localCenter.y >= 0.f)
		{
			--m_GroundContactCount;
			if (m_GroundContactCount <= 0)
			{
				m_GroundContactCount = 0;
				m_fCoyoteTimer = 0.08f;
				m_GroundNormal = Vec3(0.f, 1.f, 0.f);
			}
		}
	}

	// ─── 벽 이탈 (Layer 17) ───
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::WALL_COLLIDER)
	{
		float ownX  = _OwnCollider->GetWorldCenter().x;
		float wallX = _OtherCollider->GetWorldCenter().x;

		if (ownX >= wallX)
			m_WallContactLeft  = max(0, m_WallContactLeft - 1);
		else
			m_WallContactRight = max(0, m_WallContactRight - 1);
	}
}

void CPlayerData::Tick()
{
	if (m_IsDead)
		return;

	// 코요테 타임 처리: 타이머 만료 시 낙하 시작
	if (m_fCoyoteTimer > 0.f)
	{
		m_fCoyoteTimer -= DT;
		if (m_fCoyoteTimer <= 0.f)
		{
			m_fCoyoteTimer = 0.f;
			m_IsFalling = true;
		}
	}

	// Player 위치 갱신
	m_CurPos = GetOwner()->Transform()->GetRelativePos();
}

void CPlayerData::SaveToLevelFile(FILE* _File)
{
	// 파일로 저장해야할 멤버 정리
	// m_TargetObject, m_AnchorObject는 Begin에서 초기화 하므로 파일 저장 X

	// 파일 저장 조건을
	// 프로그램을 재실행하는가? 라는 조건을 두고
	// 먼저 재실행한다 하더라도, 저장 및 불러오기가 필요한 멤버 먼저 저장

	// m_OriginPos, m_CurPos
	// m_FullHP, m_CurHP, m_Damage, m_Speed, m_JumpVelocity
	// m_DeathCount
	//
	// 위 멤버는 재실행할때, 해당 정보를 불러와서 이어하길 원한다면 파일에 저장합니다.
	// 
	//fwrite(&m_FullHP, sizeof(float), 1, _File);
	//fwrite(&m_Damage, sizeof(float), 1, _File);
	//fwrite(&m_Speed, sizeof(float), 1, _File);
	//fwrite(&m_JumpVelocity, sizeof(float), 1, _File);

	SaveAssetRef(_File, m_EnergyBlast.Get());
}

void CPlayerData::LoadFromLevelFile(FILE* _File)
{
	m_EnergyBlast = LoadAssetRef<APrefab>(_File);
}

bool CPlayerData::GetIsAttack()
{
	bool isAttack = m_IsAttack;
	m_IsAttack = false;

	return isAttack;
}

void CPlayerData::SetIsAttack()
{
	m_IsAttack = true;
}

void CPlayerData::OffIsAttack()
{
	// false로 강제 전환 함수
	m_IsAttack = false;
}
