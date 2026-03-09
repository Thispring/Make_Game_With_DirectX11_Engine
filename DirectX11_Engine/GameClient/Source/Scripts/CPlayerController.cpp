#include "pch.h"
#include "CPlayerController.h"
#include "CPlayerStatus.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CPlayerController::CPlayerController()
	: CScript(SCRIPT_TYPE::PLAYERCONTROLLER)
	, m_pStatus(nullptr)
{
}

CPlayerController::~CPlayerController()
{
}


void CPlayerController::Begin()
{
	// CPlayerStatus 주소를 가져와 멤버 변수 접근
	// ex) Jump 함수 호출 시, Status의 멤버인 m_IsFalling을 변경
	m_pStatus = GetOwner()->GetScript<CPlayerStatus>();
}

void CPlayerController::Tick()
{
	Move();
	Jump();
	Attack();
}

void CPlayerController::SaveToLevelFile(FILE* _File)
{
}

void CPlayerController::LoadFromLevelFile(FILE* _File)
{
}

void CPlayerController::Move()
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
		// 이전 프레임에 Move 관련 Key가 눌렸는데, 현재 상태가
		// Idle이 아니면 상태를 Idle로 변경
		if (m_pStatus->GetState() != PLAYER_STATE::IDLE)
			m_pStatus->SetState(PLAYER_STATE::IDLE);
	}

	if (KEY_PRESSED(KEY::UP))
	{
		//vPos += vUp * 250.f * DT;
		if (m_pStatus->GetState() != PLAYER_STATE::U_MOVE) m_pStatus->SetState(PLAYER_STATE::U_MOVE);
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		// 추락 해제 시, 밑으로 더 내려가지 못하도록 방지
		if (!m_pStatus->GetIsFalling())
			return;

		//vPos += vDown * 250.f * DT;
		if (m_pStatus->GetState() != PLAYER_STATE::D_MOVE) m_pStatus->SetState(PLAYER_STATE::D_MOVE);
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos += vRight * 250.f * DT;
		// 방향에 따라 음수/양수 토글
		if (vScale.x < 0) vScale.x *= -1.f;

		if (m_pStatus->GetState() != PLAYER_STATE::R_MOVE) m_pStatus->SetState(PLAYER_STATE::R_MOVE);
	}
	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos += vLeft * 250.f * DT;
		// 방향에 따라 음수/양수 토글
		if (vScale.x > 0) vScale.x *= -1.f;

		if (m_pStatus->GetState() != PLAYER_STATE::L_MOVE) m_pStatus->SetState(PLAYER_STATE::L_MOVE);
	}

	// Key입력으로 변경된 정보를 Setter를 통해
	// 변경된 데이터를 반영
	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);
	GetOwner()->Transform()->SetRelativeRot(vRotation);
}

void CPlayerController::Jump()
{
	if (KEY_PRESSED(KEY::SPACE))
	{
		if (m_pStatus->GetState() != PLAYER_STATE::JUMP) m_pStatus->SetState(PLAYER_STATE::JUMP);

		m_pStatus->SetIsFalling(false);

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
		Vec3 vDown = -vUp;

		vPos += vUp * 250.f * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

	// 이전 프레임에 Jump 했다면 다시 추락중으로 변경
	if (KEY_RELEASED(KEY::SPACE))
	{
		m_pStatus->SetIsFalling(true);

		// Idle이 아니면, Idle로 변경해주기 (Flipbook)
		if (m_pStatus->GetState() != PLAYER_STATE::IDLE)
			m_pStatus->SetState(PLAYER_STATE::IDLE);
	}
}

void CPlayerController::Shoot()
{
	//if (KEY_TAP(KEY::F))
	//{
	//	if (m_MissileCount >= 1)
	//		return;

	//	m_MissileCount++;
	//	m_MissileReturn = false;

	//	SetState(PLAYER_STATE::ATTACK);

	//	/***********************************************
	//	* 미사일 발사 코드
	//	* 플레이어가 미사일을 발사해야하므로
	//	* 여기에서 미사일 게임 오브젝트를 생성
	//	*
	//	* 스마트 포인터에서 일반 포인터로 변경
	//	* TaskMgr이 생성을 담당하기 때문에, 스마트 포인터의
	//	* refCount를 변경하지 않기 위해서 일반 포인터로 생성
	//	***********************************************/
	//	GameObject* pObject = new GameObject;

	//	pObject->SetName(L"Missile");
	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CFlipbookRender);
	//	pObject->AddComponent(new CMissileScript);
	//	pObject->AddComponent(new CCollider2D);

	//	// 자식 오브젝트의 주소를 얻어와서
	//	// Pos 값을 미사일 오브젝트에게 전달
	//	Ptr<GameObject> pChild = GetOwner()->GetChild(0);

	//	Vec3 vMyPos = pChild->Transform()->GetWorldPos();
	//	Vec3 vRotation = GetOwner()->Transform()->GetRelativeRot();
	//	Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
	//	vRotation.z += XM_PI + 95.85f;


	//	// 생성되는 미사일 Pos를 PlayerScript를 가진 Pos값으로 갱신
	//	pObject->Transform()->SetRelativePos(vMyPos);
	//	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	//	pObject->Transform()->SetRelativeRot(vRotation); //(XM_PI  + 95.85f)));

	//	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotWeapon.flip"));
	//	pObject->FlipbookRender()->Play(0, 15.f, -1);

	//	pObject->GetGameObject(pChild);

	//	// 오브젝트 등록은 TaskMgr에서 담당
	//	CreateObject(pObject, 4);


	//	/***************************************************************
	//	* 두 좌표를 4분면으로 계산하여
	//	* 부호를 바꾸는 방법?
	//	*  => vRotation.z += DT * XM_PI(=Dx11에서 정의한 3.14, 파이);
	//	***************************************************************/
	//}
}

void CPlayerController::Attack()
{
	// 펀치
	if (KEY_TAP(KEY::Z))
	{
		m_pStatus->SetState(PLAYER_STATE::PUNCH);
	}

	// KEY_RELEASED 조건을 넣으면, 연속 공격처럼 Flipbook이 재생안됨
}

