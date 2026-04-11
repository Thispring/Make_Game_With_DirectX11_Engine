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
	// GameObject 멤버, 자식 세팅
	m_TargetObject = GetOwner();
	m_AnchorObject = GetOwner()->GetChild(PLAYER_PROJECTILE_ANCHOR);
	m_AnchorObject->SetLayerIdx((int)LEVEL_0_LAYER::PLAYER_PROJECTILE);
	GetOwner()->GetChild(PLAYER_KICK_ANCHOR)->SetLayerIdx((int)LEVEL_0_LAYER::PLAYER_MELEE_TRIGGER);
	GetOwner()->GetChild(PLAYER_PUNCH_ANCHOR)->SetLayerIdx((int)LEVEL_0_LAYER::PLAYER_MELEE_TRIGGER);
	
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
		SetIsDead(true);
		Ptr<CPlayerStateManager> pMgr = GetOwner()->GetScript<CPlayerStateManager>();
		pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)PLAYER_STATE::DEATH));
		pMgr->ChangeState();
	}

	int otherLayer = _OtherCollider->GetOwner()->GetLayerIdx();

    // ─── 바닥 / 천장 (Layer 16) ───
	if (otherLayer == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// 보다 견고한 위/아래 판별: 다른 콜라이더의 노멀을 사용하여 접촉 방향을 판단합니다.
		// 또한, 깊게 침투한 경우에는 침투량만큼 위치 보정하여 "끼임" 현상을 방지합니다.
		if (_OtherCollider->IsCacheDirty())
			_OtherCollider->UpdateSlopeCache();

		Matrix otherMat = _OtherCollider->GetWorldMat();
		Matrix ownMat = _OwnCollider->GetWorldMat();

		Vec3 otherNormal = _OtherCollider->GetCachedNormal();

		Vec3 vecCenter = _OwnCollider->GetWorldCenter() - _OtherCollider->GetWorldCenter();
		float distAlongNormal = XMVectorGetX(XMVector3Dot(vecCenter, otherNormal));

		// 반높이(대략) 계산: 월드 매트릭스의 Y 축 벡터 길이 기준
		float otherHalfH = Vec3(otherMat._21, otherMat._22, otherMat._23).Length() * 0.5f;
		float ownHalfH   = Vec3(ownMat._21, ownMat._22, ownMat._23).Length() * 0.5f;

		// 정상적으로 바닥 쪽에 위치해 있다면 distAlongNormal이 양수(노멀 방향)임
		if (distAlongNormal >= 0.f)
		{
          // 바닥 접촉: 점프 중일 때는 하강 중일 경우에만 착지로 처리
			if (!m_IsJumping || m_VelocityY <= 0.f)
			{
				++m_GroundContactCount;
				m_IsFalling = false;
				m_VelocityY = 0.f;
				m_GroundNormal = otherNormal;
				// 착지로 판정되면 점프 플래그를 해제
				m_IsJumping = false;

				// 침투 보정: 중심 축 거리(distAlongNormal)가 두 반높이 합보다 작으면 겹침
				float requiredCenterDist = otherHalfH + ownHalfH;
				float penetration = requiredCenterDist - distAlongNormal;
				if (penetration > 0.f)
				{
					Vec3 pos = GetOwner()->Transform()->GetRelativePos();
					pos += otherNormal * penetration;
					GetOwner()->Transform()->SetRelativePos(pos);
				}
			}
		}
		else
		{
			// 천장에 닿아서 들어온 경우: 위로 향하는 속도 차단
			if (m_VelocityY > 0.f)
				m_VelocityY = 0.f;

			// 침투 보정: 천장에 깊게 들어간 경우 약간 위치를 밀어내어 끼임/반복 충돌로
			// 인해 즉시 낙하로 전환되는 현상을 완화합니다.
			{
				float penetration = (otherHalfH + ownHalfH) - fabsf(distAlongNormal);
				if (penetration > 0.f)
				{
					// otherNormal은 표면의 바깥쪽을 향하는 법선입니다. 천장 쪽 침투는
					// distAlongNormal < 0 이므로 법선의 반대 방향으로 보정합니다.
					Vec3 pos = GetOwner()->Transform()->GetRelativePos();
					pos -= otherNormal * penetration;
					GetOwner()->Transform()->SetRelativePos(pos);
				}
			}
		}
	}

	// ─── 벽 (Layer 17) ───
	if (otherLayer == (int)LEVEL_0_LAYER::WALL_COLLIDER)
	{
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

	// ─── BACK_GROUND_COLLIDER: 천장 충돌 처리 + 접지 유지 ───
	if (otherLayer == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		Matrix invSlope  = _OtherCollider->GetWorldMat().Invert();
		Vec3 centerWorld = _OwnCollider->GetWorldCenter();
		Vec3 localCenter = XMVector3TransformCoord(centerWorld, invSlope);

        if (localCenter.y >= 0.f)
		{
			// 바닥 위에 있는 동안: 낙하 중지, 수직속도 보정(착지)
			// 단, 점프 상태에서는 착지로 처리하지 않음
			if (!m_IsJumping)
			{
				m_IsFalling = false;
				// 착지 시 아래로 향하는 속도는 0으로 (안정화)
				if (m_VelocityY < 0.f)
					m_VelocityY = 0.f;
			}


		}
		else
		{
			// 천장 처리: 머리 위치로 침투 확인하여 위로 향하는 속도 차단
			Matrix ownMat  = _OwnCollider->GetWorldMat();
			Vec3 ownYAxis  = Vec3(ownMat._21, ownMat._22, ownMat._23);

			Vec3 headWorld = centerWorld;
			headWorld.x += ownYAxis.x * 0.5f;
			headWorld.y += ownYAxis.y * 0.5f;
			headWorld.z += ownYAxis.z * 0.5f;

			Vec3 localHead = XMVector3TransformCoord(headWorld, invSlope);
			float localPenetration = localHead.y + 0.5f;
			if (localPenetration > 0.f)
			{
				if (m_VelocityY > 0.f)
					m_VelocityY = 0.f;
			}
		}
	}

	// ─── 벽 (Layer 17) ─── (unchanged)
	if (otherLayer == (int)LEVEL_0_LAYER::WALL_COLLIDER)
	{
		float ownCenterX  = _OwnCollider->GetWorldCenter().x;
		float wallCenterX = _OtherCollider->GetWorldCenter().x;

		Matrix wallMat = _OtherCollider->GetWorldMat();
		float wallHalfW = Vec3(wallMat._11, wallMat._12, wallMat._13).Length() * 0.5f;

		Matrix ownMat = _OwnCollider->GetWorldMat();
		float ownHalfW = Vec3(ownMat._11, ownMat._12, ownMat._13).Length() * 0.5f;

		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		if (ownCenterX >= wallCenterX)
		{
			float penetration = (wallCenterX + wallHalfW) - (ownCenterX - ownHalfW);
			if (penetration > 0.f)
				pos.x += penetration;
		}
		else
		{
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
		// 보다 정확한 위/아래 판별을 위해, BeginOverlap과 동일한 방식으로
		// 다른 콜라이더의 노멀을 사용하여 접촉 방향을 판단합니다.
		// 이렇게 하면 같은 레이어가 천장과 바닥을 겸할 때 천장과의 충돌
		// 이탈이 바닥 이탈로 잘못 처리되는 것을 방지합니다.
		if (_OtherCollider->IsCacheDirty())
			_OtherCollider->UpdateSlopeCache();

		Vec3 otherNormal = _OtherCollider->GetCachedNormal();
		Vec3 vecCenter = _OwnCollider->GetWorldCenter() - _OtherCollider->GetWorldCenter();
		float distAlongNormal = XMVectorGetX(XMVector3Dot(vecCenter, otherNormal));

		// distAlongNormal이 양수면 콜라이더 기준에서 플레이어가 노멀(바닥) 쪽에
		// 있었던 것이므로 바닥 이탈로 처리합니다. (천장 쪽이면 감소하지 않음)
		if (distAlongNormal >= 0.f)
		{
			--m_GroundContactCount;
			if (m_GroundContactCount <= 0)
			{
				m_GroundContactCount = 0;
				m_IsFalling = true;
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

	// Player 위치 갱신
	m_CurPos = GetOwner()->Transform()->GetRelativePos();
}

void CPlayerData::SaveToLevelFile(FILE* _File)
{
	/**************************************************************************
	* 파일로 저장해야할 멤버 정리
	* m_TargetObject, m_AnchorObject는 Begin에서 초기화 하므로 파일 저장 X
	*
	* 파일 저장 조건을
	* 프로그램을 재실행하는가? 라는 조건을 두고
	* 먼저 재실행한다 하더라도, 저장 및 불러오기가 필요한 멤버 먼저 저장
	*
	* m_OriginPos, m_CurPos
	* m_FullHP, m_CurHP, m_Damage, m_Speed, m_JumpVelocity
	* m_DeathCount
	*
	* 위 멤버는 재실행할때, 해당 정보를 불러와서 이어하길 원한다면 파일에 저장합니다.
	**************************************************************************/
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
