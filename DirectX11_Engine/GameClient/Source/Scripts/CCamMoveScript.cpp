#include "pch.h"
#include "CCamMoveScript.h"
#include "KeyMgr.h"
#include "LevelMgr.h"
#include "TimeMgr.h"
#include "CTransform.h"
#include "CCamera.h"


CCamMoveScript::CCamMoveScript()
	: CScript(SCRIPT_TYPE::CAMMOVESCRIPT)
	, m_isMoving(false)
	, m_MoveMode(CAM_MOVE_MODE::INGAME)
	, m_Target(nullptr)
{
}

CCamMoveScript::~CCamMoveScript()
{
}

void CCamMoveScript::Begin()
{
	// 처음 위치, 회전으로 돌아가는 것은 Debug 모드에서만 적용
	m_DebugOriPos = Transform()->GetRelativePos();
	m_DebugOriRot = Transform()->GetRelativeRot();

	// InGame Transform 정보는 Begin에서 초기화 후, 다른 함수에서 값 갱신
	m_InGamePos = Transform()->GetRelativePos();
	m_InGameRot = Transform()->GetRelativeRot();

	// Player 게임 오브젝트를 찾아서 등록
	//m_Target = LevelMgr::GetInst()->FindObjectByName(L"Player");
}

void CCamMoveScript::Tick()
{
	// Change Debug Cam Move Mode
	if (KEY_PRESSED(KEY::ALPHA0))
		m_MoveMode = CAM_MOVE_MODE::DEBUG;
	
	// Change InGame Cam Move Mode
	if (KEY_PRESSED(KEY::ALPHA1))
		m_MoveMode = CAM_MOVE_MODE::INGAME;


	// Debug 모드일 때만 아래 이동 함수를 실행
	if (m_MoveMode == CAM_MOVE_MODE::DEBUG)
	{
		// 원경 투영
		if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
		{
			PrespecCamMove();
			MouseCamMove();
		}
		// 직교 투영
		else if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			OrthoCamMove();
	}
	// InGame에서는 Player의 방향키 이동과 같은 속도와 방향으로 이동
	else if (m_MoveMode == CAM_MOVE_MODE::INGAME)
	{
		//Vec3 vPos = Transform()->GetRelativePos();

		//// Player의 Z축을 제외하고 가져옵니다.
		//Vec3 playerPos = m_Target->Transform()->GetRelativePos();
		//playerPos.z = vPos.z;

		//GetOwner()->Transform()->SetRelativePos(playerPos);
	}


	// RSHIFT KEY를 누르면 위치, 회전 상태 초기화
	// Resets the position and rotation when the RSHIFT KEY is pressed.
	if (KEY_PRESSED(KEY::RSHIFT))
		MoveOrigin();

	// LSHIFT KEY를 누르면 카메라의 투영방식 스위칭
	// Switches the camera's projection method when the LSHIFT KEY is pressed.
	if (KEY_TAP(KEY::LSHIFT))
		SwitchingType(Camera()->GetTypeRef());
}

void CCamMoveScript::SaveToLevelFile(FILE* _File)
{
}

void CCamMoveScript::LoadFromLevelFile(FILE* _File)
{
}

void CCamMoveScript::MoveOrigin()
{
	// 현재 자신의 위치, 회전을 원래 위치로 되돌린다.
	// Move Mode에 따라 조건 분기 합니다.

	if (m_MoveMode == CAM_MOVE_MODE::DEBUG)
	{
		Transform()->SetRelativePos(m_DebugOriPos);
		Transform()->SetRelativeRot(m_DebugOriRot);
	}
	else if (m_MoveMode == CAM_MOVE_MODE::INGAME)
	{
		Transform()->SetRelativePos(m_InGamePos);
		Transform()->SetRelativeRot(m_InGameRot);
	}
}

void CCamMoveScript::MovingOrigin()
{
	// 카메라가 서서히 이동하는 효과
	// 렌더링이 Script Tick 이후 이루어져서
	// 다른 방법을 찾아야함
}

void CCamMoveScript::OrthoCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * 500.f;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * 500.f;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * 500.f;
	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * 500.f;

	if (KEY_PRESSED(KEY::Z))
		vPos.z += DT * 250.0f;
	if (KEY_PRESSED(KEY::X))
		vPos.z -= DT * 250.0f;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
}

void CCamMoveScript::PrespecCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * 250.f;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * 250.0f;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * 250.0f;
	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * 250.0f;

	if (KEY_PRESSED(KEY::Z))
		vPos.z += DT * 250.0f;
	if (KEY_PRESSED(KEY::X))
		vPos.z -= DT * 250.0f;

	if (KEY_PRESSED(KEY::Q))
		vRot.y -= DT * XM_PI;
	if (KEY_PRESSED(KEY::E))
		vRot.y += DT * XM_PI;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}

void CCamMoveScript::MouseCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 vMouseDir = KeyMgr::GetInst()->GetMouseDir();
		vRot.y += vMouseDir.x * DT * XM_2PI * 3.f;
		vRot.x += vMouseDir.y * DT * XM_2PI * 3.f;
	}

	// 마우스 휠로 줌 구현해보기
	if (KeyMgr::GetInst()->GetMouseWheelDelta() > 0)
	{
		vPos.z += DT * XM_2PI * 3000.f;
		// 0으로 설정 하여, 매 프레임 Tick 조건에 걸리지 않도록 초기화
		KeyMgr::GetInst()->SetMouseWheelDelta(0);
	}
	else if (KeyMgr::GetInst()->GetMouseWheelDelta() < 0)
	{
		vPos.z -= DT * XM_2PI * 3000.f;
		// 0으로 설정 하여, 매 프레임 Tick 조건에 걸리지 않도록 초기화
		KeyMgr::GetInst()->SetMouseWheelDelta(0);
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}
