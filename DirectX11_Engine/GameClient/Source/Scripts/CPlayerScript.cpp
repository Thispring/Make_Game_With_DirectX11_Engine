#include "pch.h"

#include "CPlayerScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CMissileScript.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "TaskMgr.h"
#include "GameObject.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_State(PLAYER_STATE::IDLE)
	, m_StateChanged(false)
	, m_IsFalling(true)
	, m_MissileCount(0)
	, m_MissileReturn(true)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	//ADD_DYNAMIC_BEGIN_OVERLAP(CPlayerScript::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CPlayerScript::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CPlayerScript::EndOverlap);
}

void CPlayerScript::Tick()
{
	if (m_IsFalling)
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		Vec3 vDown = -vUp;

		vPos += vDown * 250.f * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

	Jump();

	/***************************************************************
	* 외부 함수에서 각 역할에 맞는 함수를 선언하고
	* Tick에서 호출하는 방법으로 코드 정리가 가능하다.
	*
	* Tick에서 TRANSFORM 을 이동하거나, 크기를 변경하는 기능은 따로 분리
	* 유니티와 언리얼 스타일이 각자 다르다.
	* 게임 오브젝트에는 TRANSFORM이 무조건 들어있어야함
	* 헷갈리면 유니티에서 GameObject 생성을 생각하기
	* -> 빈 게임 오브젝트라도 TRANSFORM이 무조건 있음
	***************************************************************/

	// Player의 자식오브젝트가 충돌했는지를 검사
	//GetOwner()->GetChild()[0]->Collider2D()->FinalTick();
	Move();
	Shoot();
	DrawDebug();

	// FlipBook 1회 재생이 끝난 상태에서, ATTACK 일때만 Tick에서 변경,
	// MOVE의 경우 Move 함수에서 KEY_RELEASED를 체크하여 IDLE로 변경하고 있음
	//if (GetOwner()->FlipbookRender()->GetFinish() == true && m_State == PLAYER_STATE::ATTACK)	// 미사일이 되돌아 오지 않았다면 ATTACK으로 유지
	if (m_State == PLAYER_STATE::ATTACK && m_MissileReturn) SetState(PLAYER_STATE::IDLE);
}

void CPlayerScript::Move()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetOwner()->Transform()->GetRelativeRot();

	Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
	Vec3 vDown = -vUp;

	Vec3 vRight = GetOwner()->Transform()->GetDir(DIR::RIGHT);
	Vec3 vLeft = -vRight;

	/**********************************************************************************
	* GetAsyncKeyState => Key가 눌렸는지 확인하는 windows 함수
	* Key의 이벤트(Pressed(눌린상태), TAP(한번눌렸을때), Released(해제), None(아무것도 아님))  
	* Key의 이벤트(상태)를 1프레임 동안 일관적으로 동작할 수 있도록 관리해야함
	**********************************************************************************/
	
	if (KEY_RELEASED(KEY::UP) || KEY_RELEASED(KEY::DOWN) || KEY_RELEASED(KEY::RIGHT) || KEY_RELEASED(KEY::LEFT))
	{
		// 이전 프레임에 Key가 눌렸다면 IDLE로 변경
		if (m_State != PLAYER_STATE::ATTACK) SetState(PLAYER_STATE::IDLE);
	}

	if (KEY_PRESSED(KEY::UP))
	{
		vPos += vUp * 250.f * DT;
		if (m_State != PLAYER_STATE::ATTACK) SetState(PLAYER_STATE::MOVE);
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		// 추락 해제 시, 밑으로 더 내려가지 못하도록 방지
		if (!m_IsFalling)
			return;

		vPos += vDown * 250.f * DT;
		if (m_State != PLAYER_STATE::ATTACK) SetState(PLAYER_STATE::MOVE);
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos += vRight * 250.f * DT;
		if (m_State != PLAYER_STATE::ATTACK) SetState(PLAYER_STATE::MOVE);
	}
	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos += vLeft * 250.f * DT;	
		if (m_State != PLAYER_STATE::ATTACK) SetState(PLAYER_STATE::MOVE);
	}
	
	// Key입력으로 변경된 정보를 Setter를 통해
	// 변경된 데이터를 반영
	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);
	GetOwner()->Transform()->SetRelativeRot(vRotation);
}

void CPlayerScript::Jump()
{
	if (KEY_PRESSED(KEY::SPACE))
	{
		m_IsFalling = false;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		Vec3 vDown = -vUp;

		vPos += vUp * 250.f * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

	if (KEY_RELEASED(KEY::SPACE))
		m_IsFalling = true;
}

void CPlayerScript::Shoot()
{
	if (KEY_TAP(KEY::F))
	{
		if (m_MissileCount >= 1)
			return;

		m_MissileCount++;
		m_MissileReturn = false;

		SetState(PLAYER_STATE::ATTACK);

		/***********************************************
		* 미사일 발사 코드
		* 플레이어가 미사일을 발사해야하므로
		* 여기에서 미사일 게임 오브젝트를 생성
		*
		* 스마트 포인터에서 일반 포인터로 변경
		* TaskMgr이 생성을 담당하기 때문에, 스마트 포인터의
		* refCount를 변경하지 않기 위해서 일반 포인터로 생성
		***********************************************/
		GameObject* pObject = new GameObject;

		pObject->SetName(L"Missile");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CFlipbookRender);
		pObject->AddComponent(new CMissileScript);
		pObject->AddComponent(new CCollider2D);

		// 자식 오브젝트의 주소를 얻어와서
		// Pos 값을 미사일 오브젝트에게 전달
		Ptr<GameObject> pChild = GetOwner()->GetChild(0);

		Vec3 vMyPos = pChild->Transform()->GetWorldPos();
		Vec3 vRotation = GetOwner()->Transform()->GetRelativeRot();
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		vRotation.z += XM_PI + 95.85f;


		// 생성되는 미사일 Pos를 PlayerScript를 가진 Pos값으로 갱신
		pObject->Transform()->SetRelativePos(vMyPos);
		pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
		pObject->Transform()->SetRelativeRot(vRotation); //(XM_PI  + 95.85f)));

		pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotWeapon.flip"));
		pObject->FlipbookRender()->Play(0, 15.f, -1);
		
		pObject->GetGameObject(pChild);

		// 오브젝트 등록은 TaskMgr에서 담당
		CreateObject(pObject, 4);


		/***************************************************************
		* 두 좌표를 4분면으로 계산하여
		* 부호를 바꾸는 방법?
		*  => vRotation.z += DT * XM_PI(=Dx11에서 정의한 3.14, 파이);
		***************************************************************/
	}
}


void CPlayerScript::DrawDebug()
{
	if (KEY_TAP(KEY::ENTER))
	{
		/***************************************************************
		* Circle Debug가 추가되지 않았던 이유는
		* 현재 Debug Draw는 World행렬 정보를 받아 그려지기 때문에
		* 지금 코드에서 WorldMat을 가져오지 않고 실행하면 Render에서 오류 발생
		***************************************************************/
		DrawDebugRect(Transform()->GetWorldMat(), Vec4(1.f, 0.f, 0.f, 1.f), 2.f);
	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// Ground와 충돌을 체크하고, 충돌했다면 추락 중지
	m_IsFalling = false;
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	// Ground와 충돌이 벗어났다면 다시 true로 변경
	m_IsFalling = true;
}
