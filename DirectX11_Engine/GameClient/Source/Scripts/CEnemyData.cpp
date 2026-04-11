#include "pch.h"
#include "CEnemyData.h"

#include "LevelMgr.h"
#include "TimeMgr.h"
#include "AssetMgr.h"

#include "CCollider2D.h"

#include "Source\Content\EnemyDamageState.h"

#include "CFlowerProjectile.h"
#include "CEnemyStateManager.h"
#include "CPlayerData.h"
#include "CPlayerMeleeTrigger.h"

vector<CEnemyData*> CEnemyData::s_AllInstances;

CEnemyData::CEnemyData()
	: CScript(SCRIPT_TYPE::ENEMYDATA)
	// 멤버들은 추후 파일로 저장하여, 불러오는 방식을 사용합니다.
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_Speed(100.f)
	, m_JumpVelocity(300.f)
	, m_VelocityY(0.f)
	, m_GroundNormal(0.f, 1.f, 0.f)
	, m_fCoyoteTimer(0.f)
	, m_GroundContactCount(0)
	, m_WallContactLeft(0)
	, m_WallContactRight(0)
	, m_Offset(0.f)
	, m_TimeSinceSpawn(0.f)
	, m_TimeInState(0.f)
	, m_fPlayerLostTimer(0.f)
	, m_bPlayerLostPending(false)

	, m_Direction(1)		// 이동방향 1로 초기화

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)
	, m_isFixedDir(false)

	, m_InitialPos{}
	, m_InitialRot{}
	, m_InitialScale{}


	, m_OriginPos{}
	, m_CurPos{}
	, m_OriginRot{}
	, m_CurRot{}

	// EnemyType은 기본으로 END입니다.
	// ImGui에서 지정해야 하며, 하지 않을경우 크래시
	, m_EnemyType(ENEMY_TYPE::END)

	, m_TargetObject(nullptr)
	, m_EyeObject(nullptr)
	, m_FlowerProjectile(nullptr)
{
}

CEnemyData::~CEnemyData()
{
	// static 리스트에서 자기 자신을 제거
	auto it = std::find(s_AllInstances.begin(), s_AllInstances.end(), this);
	if (it != s_AllInstances.end())
		s_AllInstances.erase(it);
}

void CEnemyData::Init()
{
	// Level 시작 시, 벡터를 초기화
	ClearScriptParam();

	// Init은 AddComponent 시점에 이루어짐
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FullHP, L"FullHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurHP, L"CurHP", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, L"Damage", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpVelocity, L"JumpVelocity", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_VelocityY, L"VelocityY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Offset, L"Offset", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_TimeSinceSpawn, L"TimeSinceSpawn", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_TimeInState, L"TimeInState", true, 0.f);

	// Spawn 위치가 잘 되었는지 확인용
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_InitialPos, L"InitialPos", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::VEC3_ROT, &m_InitialRot, L"InitialRot", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_InitialScale, L"InitialScale", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::INT, &m_Direction, L"Direction", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_isFixedDir, L"isFixedDir", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_FlowerProjectile, L"FlowerProjectile", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::PTR, &m_TargetObject, L"TargetObject", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PTR, &m_EyeObject, L"EyeObject", true, 0.f);

	// enum class ENEMY_TYPE을 ImGui에서 편집할 수 있도록 전달
	// Level Play 전에 m_EnemyType을 미리 받을 수 있게 보장합니다.
	AddScriptParam(SCRIPT_PARAM::ENUM_CLASS, &m_EnemyType, L"EnemyType", true, 0.f,
		vector<wstring>{ L"DEMON", L"SKULL", L"FLYING", L"FLOWER", L"BOSS", L"TEST", L"END" });
}

void CEnemyData::Begin()
{
	// NOTE(26-03-31):
	// 문자열로 오브젝트를 지정하는 방식은 
	// Enemy가 게임에 많이 스폰되는 경우
	// 문자열이 같다고 보장 할 수 없습니다. 다른 방식 찾아보기
	// 
	// 조건문으로 타입을 정하는것이 아닌, 인스펙터에서
	// 타입을 지정하고, 이를 저장 및 불러오는 방식으로 변경하기
	m_TargetObject = GetOwner();
	m_EyeObject = GetOwner()->GetChild(ENEMY_EYES).Get();
	m_EyeObject->SetLayerIdx(9);

	// assert는 조건이 false일 때만 실행(중단)됩니다.
	assert(m_EnemyType != ENEMY_TYPE::END && "EnemyType is End");

	// FLOWER 타입의 경우, ENEMY_PROJECTILE_ANCHOR 자식을 다른 Layer로 설정
	// Player 공격범위에서 제외하기 위함
	if (m_EnemyType == ENEMY_TYPE::FLOWER)
	{
		GetOwner()->GetChild(ENEMY_PROJECTILE_ANCHOR)->SetLayerIdx((int)LEVEL_0_LAYER::ENEMY_PROJECT_ANCHOR);
	}

	// m_InitialPos로 초기위치 정보를 설정
	GetOwner()->Transform()->SetRelativePos(m_InitialPos);
	GetOwner()->Transform()->SetRelativeRot(m_InitialRot);
	GetOwner()->Transform()->SetRelativeScale(m_InitialScale);


	// 기존 위치는 Begin에서 초기화
	m_OriginPos = GetOwner()->Transform()->GetRelativePos();
	m_CurPos = m_OriginPos;

	m_OriginRot = GetOwner()->Transform()->GetRelativeRot();
	m_CurRot = m_OriginRot;


	// 스케일 x축값을 읽어와 음수인지 양수인지 판단하여 이동방향을 미리 결정합니다.
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	if (vScale.x > 0)
		m_Direction = 1;
	else
		m_Direction = -1;

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyData::EndOverlap);

	// m_TargetObject에 같이 있는 CEnemyStateManager 객체의 주소를 얻어와
	// SetUp 함수 호출, 호출 순서를 보장하기 위함
	// Script의 Begin을 호출한다면, 등록된 순서대로 각 콘텐츠 스크립트의 Begin이 호출되기 때문에
	// State 생성 부분을 m_EnemyType이 결정된 이후 호출하게 합니다.
	m_TargetObject->GetScript<CEnemyStateManager>()->SetUp();


	//===========================================================
	// static 리스트에 자기 자신 등록 (중복 방지)
	// 비활성화되어도 리스트에는 남아있어, GameMgr에서 직접 접근 가능
	// 
	// Begin에 등록하는 이유는, Level 시작 시에 등록하면 되기 때문
	// Init은 Level 시작 여부와 상관없이 호출
	//===========================================================
	auto it = find(s_AllInstances.begin(), s_AllInstances.end(), this);
	if (it == s_AllInstances.end())
		s_AllInstances.push_back(this);

}

void CEnemyData::TakeDamage(float _Damage, bool _hitSkull)
{
	// 해당 함수를 Player쪽 공격 스크립트에서 호출하고 있기 때문에
	// HIT 상태 변경을 이쪽에서 처리합니다.

	// 죽은 상태에서 함수가 또 호출되면, HIT 상태로 되돌리기 X
	if (m_IsDead)
		return;

	// BOSS 타입이면 데미지 입는 판정 X
	if (m_EnemyType == ENEMY_TYPE::BOSS)
		return;

	// 이미 GHOST_SKULL 상태라면 전환 X
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	ENEMY_STATE curState = pMgr->GetCurCommonState();
	if (curState == ENEMY_STATE::GHOST_SKULL || curState == ENEMY_STATE::GHOST_SKULL_MOVE)
		return;

	// 구독자(AttackState 등)에게 피격 사실을 즉시 알림
	//  => 공격 중, 피격 당했다면 바로 HIT로 넘어가게 하는 기능
	if (m_OnTakeDamageEvent)
		m_OnTakeDamageEvent();

	if (m_EnemyType == ENEMY_TYPE::SKULL && _hitSkull == true)
	{
		// SKULL타입만 무적상태로 진입
		ChangeState(ENEMY_STATE::GHOST_SKULL);
		return;
	}

	// 실제 데미지 처리는 CEnemyData에서 진행
	// FSM 설계상 여기에서 진행하면 안됨
	float hp = GetCurHP();
	hp -= _Damage;
	SetCurHP(hp);

	ChangeState(ENEMY_STATE::HIT);

	if (hp <= 0.f)
	{
		// Dead 상태 호출
		m_IsDead = true;
		ChangeState(ENEMY_STATE::DEAD);
	}
}

void CEnemyData::ChangeState(ENEMY_STATE _State)
{
	//=========================================
	// CEnemyData 클래스에서만 사용할 상태 변경 함수
	//=========================================

	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();

	// map에서 해당 ENEMY_STATE 키가 존재하는지 확인하고 포인터를 반환
	EnemyState* pState = pMgr->GetStatusByCommonState(_State);
	if (pState == nullptr)
	{
		assert(false && "ChangeState: Requested ENEMY_STATE not found in map");
		return;
	}

	pMgr->SetCurStatus(pState);
	pMgr->ChangeState();
}

void CEnemyData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// 수정: FLYING/FLOWER 타입인 경우 'BACK_GROUND_COLLIDER' 에 대해서만
	// 바닥/천장 관련 로직을 무시하도록 변경했습니다.
	int otherLayer = _OtherCollider->GetOwner()->GetLayerIdx();

	// 추락 시, 사망 처리 (OutOfBounds은 항상 처리)
	if (otherLayer == (int)LEVEL_0_LAYER::OUT_OF_BOUNDS)
	{
		// Dead 상태 호출
		m_IsDead = true;
		ChangeState(ENEMY_STATE::DEAD);
		return;
	}

	// ─── 바닥 / 천장 (Layer 16) ───
	if (otherLayer == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// FLYING, FLOWER 타입은 바닥 충돌로 인한 복원 로직 적용 X
		if (m_EnemyType == ENEMY_TYPE::FLYING || m_EnemyType == ENEMY_TYPE::FLOWER)
			return;

		Matrix invSlope = _OtherCollider->GetWorldMat().Invert();
		Vec3 localCenter = XMVector3TransformCoord(_OwnCollider->GetWorldCenter(), invSlope);

		++m_GroundContactCount;
		m_IsFalling = false;
		m_fCoyoteTimer = 0.f;

		Matrix slopeMat = _OtherCollider->GetWorldMat();
		Vec3 normal = Vec3(slopeMat._21, slopeMat._22, slopeMat._23);
		normal.Normalize();
		m_GroundNormal = (fabsf(normal.y) > 0.99f) ? Vec3(0.f, 1.f, 0.f) : normal;
	}

	// ─── 벽 (Layer 17, 12) ───
	if (otherLayer == (int)LEVEL_0_LAYER::WALL_COLLIDER ||
		otherLayer == (int)LEVEL_0_LAYER::ENEMY_WALL_COLLIDER)
	{
		float ownX = _OwnCollider->GetWorldCenter().x;
		float wallX = _OtherCollider->GetWorldCenter().x;

		if (ownX >= wallX)
			++m_WallContactLeft;
		else
			++m_WallContactRight;
	}
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_IsDead == true)
		return;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// FLYING, FLOWER 타입은 로직 적용 X
		if (m_EnemyType == ENEMY_TYPE::FLYING || m_EnemyType == ENEMY_TYPE::FLOWER)
			return;

		// 자신 콜라이더의 월드 Y축 (스케일 포함)
		Matrix ownMat = _OwnCollider->GetWorldMat();
		Vec3 ownYAxis = Vec3(ownMat._21, ownMat._22, ownMat._23);

		Vec3 centerWorld = _OwnCollider->GetWorldCenter();
		Matrix invSlope = _OtherCollider->GetWorldMat().Invert();
		Vec3 localCenter = XMVector3TransformCoord(centerWorld, invSlope);

		// ─── 바닥 ───
		Vec3 footWorld = centerWorld;
		footWorld.x -= ownYAxis.x * 0.5f;
		footWorld.y -= ownYAxis.y * 0.5f;
		footWorld.z -= ownYAxis.z * 0.5f;

		Vec3 localFoot = XMVector3TransformCoord(footWorld, invSlope);
		float localPenetration = 0.5f - localFoot.y;

		if (localPenetration > 0.f)
		{
			Matrix slopeMat = _OtherCollider->GetWorldMat();
			Vec3   pos = GetOwner()->Transform()->GetRelativePos();
			pos.x += localPenetration * slopeMat._21;
			pos.y += localPenetration * slopeMat._22;
			GetOwner()->Transform()->SetRelativePos(pos);
		}
	}

	// ─── 벽 (Layer 17, 12) ───
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::WALL_COLLIDER ||
		_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::ENEMY_WALL_COLLIDER)
	{
		// FLYING, FLOWER 타입은 벽에 막히는 로직 적용 X
		if (m_EnemyType == ENEMY_TYPE::FLYING || m_EnemyType == ENEMY_TYPE::FLOWER)
			return;

		float ownCenterX = _OwnCollider->GetWorldCenter().x;
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

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// FLOWER 타입이면 ATTACK(근접공격)으로 진입하고 return
		if (m_EnemyType == ENEMY_TYPE::FLOWER)
		{
			ChangeState(ENEMY_STATE::ATTACK);
			return;
		}

		// 플레이어가 다시 들어오면 그레이스 타임 취소
		m_bPlayerLostPending = false;
		m_fPlayerLostTimer = 0.f;

		Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
		ENEMY_STATE curState = pMgr->GetCurCommonState();

		// HIT / DEAD / GHOST 상태에서는 ATTACK으로 강제 전환하지 않음
		if (curState == ENEMY_STATE::HIT
			|| curState == ENEMY_STATE::DEAD
			|| curState == ENEMY_STATE::GHOST_SKULL
			|| curState == ENEMY_STATE::GHOST_SKULL_MOVE)
			return;

		ChangeState(ENEMY_STATE::ATTACK);
	}
}

void CEnemyData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// HIT, DEAD 상태에서는 IDLE로 강제 전환하지 않음
	// 해당 상태들은 Flipbook 재생 완료 후 자체적으로 전이합니다.
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	ENEMY_STATE curState = pMgr->GetCurCommonState();

	if (curState == ENEMY_STATE::HIT || curState == ENEMY_STATE::DEAD)
		return;

	// 수정: SKULL + GHOST_SKULL 계열이면 이탈 로직 적용 X (OR로 검사)
	if (m_EnemyType == ENEMY_TYPE::SKULL &&
		(curState == ENEMY_STATE::GHOST_SKULL || curState == ENEMY_STATE::GHOST_SKULL_MOVE))
	{
		return;
	}

	int otherLayer = _OtherCollider->GetOwner()->GetLayerIdx();

	// 플레이어 이탈 처리: Eyes와 동일한 범위로 처리 (Eyes 쪽과 시그널 충돌이 발생하지 않도록)
	if (otherLayer == (int)LEVEL_0_LAYER::PLAYER)
	{
		// FLOWER 타입 예외: RANGED_ATTACK -> IDLE 전환 처리
		if (m_EnemyType == ENEMY_TYPE::FLOWER)
		{
			if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::RANGED_ATTACK))
			{
				pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
				pMgr->ChangeState();
			}
			return;
		}

     // 일반 엔진은 CHASE 상태일 때만 IDLE로 전환
		if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::CHASE))
		{
			pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
			pMgr->ChangeState();
		}
		else
		{
			// 플레이어가 ATTACK/RANGED_ATTACK 상태에서 이탈했을 경우 즉시 Idle로 가지 않도록
			// 짧은 그레이스 타임을 적용: 일정 시간 대기 후 IDLE로 전환
			if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::ATTACK) ||
				pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::RANGED_ATTACK))
			{
				m_bPlayerLostPending = true;
				m_fPlayerLostTimer = 0.15f; // 그레이스 타임 (초)
			}
		}

		return;
	}

	// ─── 바닥 이탈 (Layer 16) ───
	if (otherLayer == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// FLYING, FLOWER 타입은 로직 적용 X
		if (m_EnemyType == ENEMY_TYPE::FLYING || m_EnemyType == ENEMY_TYPE::FLOWER)
			return;

		Matrix invSlope = _OtherCollider->GetWorldMat().Invert();
		Vec3 localCenter = XMVector3TransformCoord(_OwnCollider->GetWorldCenter(), invSlope);

		--m_GroundContactCount;
		if (m_GroundContactCount <= 0)
		{
			m_GroundContactCount = 0;
			m_fCoyoteTimer = 0.08f;
			m_GroundNormal = Vec3(0.f, 1.f, 0.f);
		}
	}

	// ─── 벽 이탈 (Layer 17, 12) ───
	if (otherLayer == (int)LEVEL_0_LAYER::WALL_COLLIDER ||
		otherLayer == (int)LEVEL_0_LAYER::ENEMY_WALL_COLLIDER)
	{
		// FLYING, FLOWER 타입은 로직 적용 X
		if (m_EnemyType == ENEMY_TYPE::FLYING || m_EnemyType == ENEMY_TYPE::FLOWER)
			return;

		float ownX = _OwnCollider->GetWorldCenter().x;
		float wallX = _OtherCollider->GetWorldCenter().x;

		if (ownX >= wallX)
			m_WallContactLeft = max(0, m_WallContactLeft - 1);
		else
			m_WallContactRight = max(0, m_WallContactRight - 1);
	}
}


void CEnemyData::CreateProjectile()
{
	Ptr<APrefab> pProjectile = GetFlowerProjectile();

	// FLOWER 에게만 2번째 자식으로 Anchor 추가
	// Anchor의 Pos, Scale을 가져와서 Prefab 시작점으로 설정
	Vec3 vAnchorPos = GetTargetObject()->GetChild(ENEMY_PROJECTILE_ANCHOR)->Transform()->GetWorldPos();
	Vec3 vAnchorScale = GetTargetObject()->GetChild(ENEMY_PROJECTILE_ANCHOR)->Transform()->GetWorldScale();

	Vec3 vDir = GetTargetObject()->Transform()->GetDir(DIR::RIGHT);
	vDir *= GetDirection();

	GameObject* pObj = InstantiateObject(pProjectile.Get(), (int)LEVEL_0_LAYER::ENEMY_PROJECTILE, vAnchorPos + vAnchorScale * vDir);
	pObj->GetScript<CFlowerProjectile>()->SetUp(vDir);
}

void CEnemyData::Tick()
{
	if (m_IsDead == true)
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

	// 소환 후 흐른 시간 계산
	m_TimeSinceSpawn += DT;

	// 플레이어 이탈 그레이스 타임 처리
	if (m_bPlayerLostPending)
	{
		m_fPlayerLostTimer -= DT;
		if (m_fPlayerLostTimer <= 0.f)
		{
			m_bPlayerLostPending = false;
			m_fPlayerLostTimer = 0.f;
			// 상태 확인 후 IDLE로 전환
			Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
			if (pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::ATTACK) ||
				pMgr->GetCurStatus() == pMgr->GetStatusByIndex((int)ENEMY_STATE::RANGED_ATTACK))
			{
				pMgr->SetCurStatus(pMgr->GetStatusByIndex((int)ENEMY_STATE::IDLE));
				pMgr->ChangeState();
			}
		}
	}
}

void CEnemyData::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
	SaveAssetRef(_File, m_FlowerProjectile.Get());

	fwrite(&m_InitialPos, sizeof(Vec3), 1, _File);
	fwrite(&m_InitialRot, sizeof(Vec3), 1, _File);
	fwrite(&m_InitialScale, sizeof(Vec3), 1, _File);
	fwrite(&m_isFixedDir, sizeof(bool), 1, _File);
}

void CEnemyData::LoadFromLevelFile(FILE* _File)
{
	fread(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
	m_FlowerProjectile = LoadAssetRef<APrefab>(_File);

	fread(&m_InitialPos, sizeof(Vec3), 1, _File);
	fread(&m_InitialRot, sizeof(Vec3), 1, _File);
	fread(&m_InitialScale, sizeof(Vec3), 1, _File);
	fread(&m_isFixedDir, sizeof(bool), 1, _File);
}


//=====================
// Enemy Reset 관련 함수
//=====================
void CEnemyData::ResetToInitial()
{
	//==========================================================
	// 개별 Enemy의 모든 상태를 Begin 직후 상태로 복원합니다.
	// GameObject가 비활성 상태여도 직접 호출되므로 Tick에 의존하지 않습니다.
	//==========================================================

	// 1. GameObject 본인 + 모든 자식 재활성화
	SetActiveRecursive(GetOwner(), true);

	// 2. HP 및 상태 플래그 초기화
	m_CurHP = m_FullHP;
	m_IsDead = false;
	m_IsFalling = true;
	m_IsAttack = false;
	m_VelocityY = 0.f;
	m_fCoyoteTimer = 0.f;
	m_GroundContactCount = 0;
	m_WallContactLeft = 0;
	m_WallContactRight = 0;
	m_GroundNormal = Vec3(0.f, 1.f, 0.f);
	m_TimeSinceSpawn = 0.f;
	m_TimeInState = 0.f;

	// 플레이어 이탈 그레이스 초기화
	m_fPlayerLostTimer = 0.f;
	m_bPlayerLostPending = false;

	// 3. 위치/회전/스케일을 초기값으로 복원
	GetOwner()->Transform()->SetRelativePos(m_InitialPos);
	GetOwner()->Transform()->SetRelativeRot(m_OriginRot);
	GetOwner()->Transform()->SetRelativeScale(m_InitialScale);

	m_CurPos = m_InitialPos;
	m_CurRot = m_OriginRot;

	// 4. 스케일로 방향 재결정
	if (m_InitialScale.x > 0)
		m_Direction = 1;
	else
		m_Direction = -1;

	// 5. GHOST_SKULL이었던 경우 원래 타입으로 복원
	if (m_EnemyType == ENEMY_TYPE::GHOST_SKULL)
		m_EnemyType = ENEMY_TYPE::SKULL;

	// 6. IDLE 상태로 전이
	ChangeState(ENEMY_STATE::IDLE);
}

void CEnemyData::ResetAllEnemies()
{
	//==========================================================
	// static 함수: 등록된 모든 CEnemyData 인스턴스를 순회하며 리셋
	// GameMgr은 개별 Enemy 객체를 알 필요 없이 이 함수 하나로 트리거
	//==========================================================
	for (CEnemyData* pEnemy : s_AllInstances)
	{
		if (pEnemy != nullptr)
			pEnemy->ResetToInitial();
	}
}

void CEnemyData::ClearAllInstances()
{
	//==========================================================
	// Level 전환 시 호출하여 static 리스트를 비웁니다.
	// ClearLevelPlay() 등에서 사용
	//==========================================================
	s_AllInstances.clear();
}

void CEnemyData::SetActiveRecursive(GameObject* _Obj, bool _IsActive)
{
	//==========================================================
	// 자기 자신을 활성/비활성화 한 뒤,
	// 보유한 모든 자식 오브젝트를 재귀적으로 동일하게 설정합니다.
	// 자식이 다른 Layer에 있어도 m_vecChild로 접근하므로 Layer 무관
	//==========================================================
	_Obj->SetIsActive(_IsActive);

	const vector<Ptr<GameObject>>& vecChild = _Obj->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		SetActiveRecursive(vecChild[i].Get(), _IsActive);
	}
}