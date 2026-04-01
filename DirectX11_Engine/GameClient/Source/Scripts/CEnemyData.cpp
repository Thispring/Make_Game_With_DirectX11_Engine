#include "pch.h"
#include "CEnemyData.h"
#include "CEnemyStateManager.h"
#include "Source\Content\EnemyDamageState.h"
#include "CCollider2D.h"
#include "LevelMgr.h"
#include "CPlayerData.h"
#include "CPlayerMeleeTrigger.h"

CEnemyData::CEnemyData()
	: CScript(SCRIPT_TYPE::ENEMYDATA)
	// 멤버들은 추후 파일로 저장하여, 불러오는 방식을 사용합니다.
	, m_FullHP(10.f)
	, m_CurHP(m_FullHP)
	, m_Damage(2.f)
	, m_JumpVelocity(300.f)
	, m_Speed(100.f)

	, m_IsDead(false)
	, m_IsFalling(true)
	, m_IsAttack(false)

	, m_OriginPos{}
	, m_CurPos{}

	, m_VelocityY(0.f)

	// EnemyType은 기본으로 END입니다.
	// ImGui에서 지정해야 하며, 하지 않을경우 크래시
	, m_EnemyType(ENEMY_TYPE::END)
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

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsDead, L"IsDead", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsFalling, L"IsFalling", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAttack, L"IsAttack", true, 0.f);

	// enum class ENEMY_TYPE을 ImGui에서 편집할 수 있도록 전달
	// Level Play 전에 m_EnemyType을 미리 받을 수 있게 보장합니다.
	AddScriptParam(SCRIPT_PARAM::ENUM_CLASS, &m_EnemyType, L"EnemyType", true, 0.f,
		vector<wstring>{ L"DEMON", L"SKULL", L"FLYING", L"FLOWER", L"BOSS", L"END" });
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
	
	// assert는 조건이 false일 때만 실행(중단)됩니다.
	assert(m_EnemyType != ENEMY_TYPE::END && "EnemyType is End");

	// 기존 위치는 Begin에서 초기화
	m_OriginPos = Vec3(0.f, 0.f, 0.f);
	m_CurPos = m_OriginPos;

	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyData::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyData::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyData::EndOverlap);

	// m_TargetObject에 같이 있는 CEnemyStateManager 객체의 주소를 얻어와
	// SetUp 함수 호출, 호출 순서를 보장하기 위함
	// Script의 Begin을 호출한다면, 등록된 순서대로 각 콘텐츠 스크립트의 Begin이 호출되기 때문에
	// State 생성 부분을 m_EnemyType이 결정된 이후 호출하게 합니다.
	m_TargetObject->GetScript<CEnemyStateManager>()->SetUp();
}

void CEnemyData::ApplyDamage(float _Damage)
{
	// 해당 함수를 Player쪽 공격 스크립트에서 호출하고 있기 때문에
	// HIT 상태 변경을 이쪽에서 처리합니다.


	// 실제 데미지 처리는 CEnemyData에서 진행
	// FSM 설계상 여기에서 진행하면 안됨
	float hp = GetCurHP();
	hp -= _Damage;
	SetCurHP(hp);

	// Type에 따라 구별되는 ENEMY_STATE를 반환
	int state = (int)GetEnemyStateToParam(m_EnemyType, ENEMY_COMMON_STATE::HIT);
	// 현재 상태를 얻어오고, 해당 상태를 세팅
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusByIndex(state));
	pMgr->ChangeState();

	if (hp <= 0.f)
	{
		// Dead 상태 호출
		m_IsDead = true;
		int state = (int)GetEnemyStateToParam(m_EnemyType, ENEMY_COMMON_STATE::DEAD);
		Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
		pMgr->SetCurStatus(pMgr->GetStatusByIndex(state));
		pMgr->ChangeState();
	}
}

void CEnemyData::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// Layer Index 4번은 Player 근접공격
	if (_OtherCollider->GetOwner()->GetLayerIdx() == 5)
	{
		// 죽었다면 HIT 상태로 되돌리기 X
		if (m_IsDead)
			return;
	}
}

void CEnemyData::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = false;
}

void CEnemyData::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	m_IsFalling = true;

	// 죽었다면 IDLE 상태로 되돌리기 X
	if (m_IsDead)
		return;

	// 중단이 걸렸으므로 여기에서 Idle로 상태변경
	int state = (int)GetEnemyStateToParam(m_EnemyType, ENEMY_COMMON_STATE::IDLE);
	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	pMgr->SetCurStatus(pMgr->GetStatusByIndex(state));
	pMgr->ChangeState();

	//// Player 투사체와 충돌 후 호출되는지 확인
	//if (_OtherCollider->GetOwner()->GetLayerIdx() == 5)
	//{
	//	// 죽었다면 IDLE 상태로 되돌리기 X
	//	if (m_IsDead)
	//		return;

	//	// 중단이 걸렸으므로 여기에서 Idle로 상태변경
	//	int state = (int)GetEnemyStateToParam(m_EnemyType, ENEMY_COMMON_STATE::IDLE);
	//	Ptr<CEnemyStateManager> pMgr = m_TargetObject->GetScript<CEnemyStateManager>();
	//	pMgr->SetCurStatus(pMgr->GetStatusByIndex(state));
	//	pMgr->ChangeState();

	//	// 근접공격이 끝났으므로 비활성화 신호 보내기
	//}
}

void CEnemyData::Tick()
{

}

void CEnemyData::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
}

void CEnemyData::LoadFromLevelFile(FILE* _File)
{
	fread(&m_EnemyType, sizeof(ENEMY_TYPE), 1, _File);
}
