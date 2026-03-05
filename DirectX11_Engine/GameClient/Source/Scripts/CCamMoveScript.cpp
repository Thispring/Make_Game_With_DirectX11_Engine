#include "pch.h"
#include "CCamMoveScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CTransform.h"
#include "CCamera.h"


CCamMoveScript::CCamMoveScript()
	: CScript(SCRIPT_TYPE::CAMMOVESCRIPT)
	, m_isMoving(false)
{
}

CCamMoveScript::~CCamMoveScript()
{
}

void CCamMoveScript::Begin()
{
	// 처음 위치, 회전 저장
	m_OriginPos = Transform()->GetRelativePos();
	m_OriginRot = Transform()->GetRelativeRot();
}

void CCamMoveScript::Tick()
{
	// 매 프레임 마다 현재 위치, 회전을 갱신
	if(!m_isMoving)
	{
		m_CurPos = Transform()->GetRelativePos();
		m_CurRot = Transform()->GetRelativeRot();
	}

	// 원경 투영
	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
	{
		PrespecCamMove();
		MouseCamMove();
	}
	// 직교 투영
	else if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
		OrthoCamMove();


	// SPACE KEY를 누르면 위치, 회전 상태 초기화
	// Resets the position and rotation when the SPACE KEY is pressed.
	if (KEY_PRESSED(KEY::SPACE))
		MoveOrigin();

	// LSHIFT KEY를 누르면 카메라의 투영방식 스위칭
	// Switches the camera's projection method when the LSHIFT KEY is pressed.
	if (KEY_TAP(KEY::LSHIFT))
		SwitchingType(Camera()->GetTypeRef());
}

void CCamMoveScript::MoveOrigin()
{
	// 현재 자신의 위치, 회전을 원래 위치로 되돌린다.
	Transform()->SetRelativePos(m_OriginPos);
	Transform()->SetRelativeRot(m_OriginRot);
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
