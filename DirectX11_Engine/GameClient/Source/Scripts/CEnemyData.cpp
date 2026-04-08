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
	, m_Offset(0.f)
	, m_TimeSinceSpawn(0.f)
	, m_TimeInState(0.f)

	, m_Direction(1)		// 이동방향 1로 초기화

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)

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
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_InitialRot, L"InitialRot", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_InitialScale, L"InitialScale", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::INT, &m_Direction, L"Direction", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

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
	m_CurPos    = m_OriginPos;

	m_OriginRot = GetOwner()->Transform()->GetRelativeRot();
	m_CurRot    = m_OriginRot;


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
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		m_fCoyoteTimer = 0.f;

		// 월드 행렬 2행(row 1) = local Y축의 월드 방향 = 접지면 법선
		Matrix slopeMat = _OtherCollider->GetWorldMat();
		Vec3 normal = Vec3(slopeMat._21, slopeMat._22, slopeMat._23);
		normal.Normalize();

		m_GroundNormal = (fabsf(normal.y) > 0.99f) ? Vec3(0.f, 1.f, 0.f) : normal;
	}
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (m_IsDead == true)
	{
		// NOTE(26-04-02):
		// IsDead 리턴 처리를 해야, 겹쳐있을 때 enemy가 Dead 상태가 되어도
		// 다시 부활하는 것 같은 현상 방지할 수 있음, 단 짧은 시간내에
		// Overlap 판정으로 인한 Attack 상태 활성화로, Player가 데미지를 입는 현상이
		// 발견되었으므로, 필요하다면 데미지 처리를 막는 로직 작성
		return;
	}

	m_IsFalling = false;

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// 자신 콜라이더의 월드 Y축 (스케일 포함)
		Matrix ownMat  = _OwnCollider->GetWorldMat();
		Vec3 ownYAxis  = Vec3(ownMat._21, ownMat._22, ownMat._23);

		// 발 위치 = 콜라이더 중심 - Y축 * 0.5f
		Vec3 footWorld = _OwnCollider->GetWorldCenter();
		footWorld.x   -= ownYAxis.x * 0.5f;
		footWorld.y   -= ownYAxis.y * 0.5f;
		footWorld.z   -= ownYAxis.z * 0.5f;

		// 발 위치를 경사면 로컬 공간으로 변환
		Matrix invSlope       = _OtherCollider->GetWorldMat().Invert();
		Vec3   localFoot      = XMVector3TransformCoord(footWorld, invSlope);
		float  localPenetration = 0.5f - localFoot.y;

		if (localPenetration > 0.f)
		{
			Matrix slopeMat = _OtherCollider->GetWorldMat();
			Vec3   pos      = GetOwner()->Transform()->GetRelativePos();
			pos.x += localPenetration * slopeMat._21;
			pos.y += localPenetration * slopeMat._22;
			GetOwner()->Transform()->SetRelativePos(pos);
		}
	}

	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::PLAYER)
	{
		// FLOWER 타입이면 return
		if (m_EnemyType == ENEMY_TYPE::FLOWER)
		{
			ChangeState(ENEMY_STATE::ATTACK);
			return;
		}

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
	if (_OtherCollider->GetOwner()->GetLayerIdx() == (int)LEVEL_0_LAYER::BACK_GROUND_COLLIDER)
	{
		// 지면 이탈: 코요테 타임으로 유예
		m_fCoyoteTimer = 0.08f;
		m_GroundNormal = Vec3(0.f, 1.f, 0.f);
	}
	else
	{
		m_IsFalling = true;
	}

	// FLOWER 타입이면 return
	if (m_EnemyType == ENEMY_TYPE::FLOWER)
	{
		ChangeState(ENEMY_STATE::IDLE);
		return;
	}

	// HIT, DEAD 상태에서는 IDLE로 강제 전환하지 않음
	// 해당 상태들은 Flipbook 재생 완료 후 자체적으로 전이합니다.
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	ENEMY_STATE curState = pMgr->GetCurCommonState();

	if (curState == ENEMY_STATE::HIT || curState == ENEMY_STATE::DEAD)
		return;

	// GHOST_SKULL이면 IDLE 전환 X
	if (curState != ENEMY_STATE::GHOST_SKULL && curState != ENEMY_STATE::GHOST_SKULL_MOVE)
		ChangeState(ENEMY_STATE::IDLE);
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
}

void CEnemyData::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
	SaveAssetRef(_File, m_FlowerProjectile.Get());
}

void CEnemyData::LoadFromLevelFile(FILE* _File)
{
	fread(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
	m_FlowerProjectile = LoadAssetRef<APrefab>(_File);
}
