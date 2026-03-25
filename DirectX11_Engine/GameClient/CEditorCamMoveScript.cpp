#include "pch.h"
#include "CEditorCamMoveScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "LevelMgr.h"

CEditorCamMoveScript::CEditorCamMoveScript()
	: CScript(-1)	// Engine 용 Script이기 때문에 따로 처리
	, m_isMoving(false)
	
	// 매 프레임 저장할 현재 Transform 정보는 생성 시점에 초기화
	, m_CurPos{}
	, m_CurRot{}
{
}

CEditorCamMoveScript::~CEditorCamMoveScript()
{
}

void CEditorCamMoveScript::Begin()
{
	// 처음 위치, 회전 저장
	m_OriginPos = Transform()->GetRelativePos();
	m_OriginRot = Transform()->GetRelativeRot();
}

void CEditorCamMoveScript::Tick()
{
	// Level Play가 아닐때만 작동
	if (LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
		return;

	// 매 프레임 마다 현재 위치, 회전을 갱신
	if (!m_isMoving)
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
	if (KEY_PRESSED(KEY::O))
		MoveOrigin();

	// LSHIFT KEY를 누르면 카메라의 투영방식 스위칭
	// Switches the camera's projection method when the LSHIFT KEY is pressed.
	if (KEY_TAP(KEY::F2))
		SwitchingType(Camera()->GetTypeRef());
}

void CEditorCamMoveScript::SaveToLevelFile(FILE* _File)
{
}

void CEditorCamMoveScript::LoadFromLevelFile(FILE* _File)
{
}

void CEditorCamMoveScript::MoveOrigin()
{
	// 현재 자신의 위치, 회전을 원래 위치로 되돌린다.
	Transform()->SetRelativePos(m_OriginPos);
	Transform()->SetRelativeRot(m_OriginRot);
}

void CEditorCamMoveScript::MovingOrigin()
{
	// 카메라가 서서히 이동하는 효과
	// 렌더링이 Script Tick 이후 이루어져서
	// 다른 방법을 찾아야함
}

void CEditorCamMoveScript::OrthoCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();

    // Use WASD for editor ortho camera movement
	if (KEY_PRESSED(KEY::W))
		vPos.y += E_DT * 500.f;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= E_DT * 500.f;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= E_DT * 500.f;
	if (KEY_PRESSED(KEY::D))
		vPos.x += E_DT * 500.f;

	if (KEY_PRESSED(KEY::Q))
		vPos.z += E_DT * 250.0f;
	if (KEY_PRESSED(KEY::E))
		vPos.z -= E_DT * 250.0f;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
}

void CEditorCamMoveScript::PrespecCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

    // Use WASD for editor perspective camera movement
	if (KEY_PRESSED(KEY::W))
		vPos.y += E_DT * 250.f;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= E_DT * 250.0f;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= E_DT * 250.0f;
	if (KEY_PRESSED(KEY::D))
		vPos.x += E_DT * 250.0f;

	if (KEY_PRESSED(KEY::Q))
		vPos.z += E_DT * 250.0f;
	if (KEY_PRESSED(KEY::E))
		vPos.z -= E_DT * 250.0f;


	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}

void CEditorCamMoveScript::MouseCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 vMouseDir = KeyMgr::GetInst()->GetMouseDir();
		vRot.y += vMouseDir.x * E_DT * XM_2PI * 3.f;
		vRot.x += vMouseDir.y * E_DT * XM_2PI * 3.f;
	}

	// 마우스 휠로 줌 구현해보기
	if (KeyMgr::GetInst()->GetMouseWheelDelta() > 0)
	{
		vPos.z += E_DT * XM_2PI * 3000.f;
		// 0으로 설정 하여, 매 프레임 Tick 조건에 걸리지 않도록 초기화
		KeyMgr::GetInst()->SetMouseWheelDelta(0);
	}
	else if (KeyMgr::GetInst()->GetMouseWheelDelta() < 0)
	{
		vPos.z -= E_DT * XM_2PI * 3000.f;
		// 0으로 설정 하여, 매 프레임 Tick 조건에 걸리지 않도록 초기화
		KeyMgr::GetInst()->SetMouseWheelDelta(0);
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}

Vec3 CEditorCamMoveScript::Vec3Abs(const Vec3& v)
{
	// Vec3의 각 요소의 절댓값을 계산하는 함수

	return {
	fabsf(v.x), // float의 절댓값은 fabsf() 사용
	fabsf(v.y),
	fabsf(v.z)
	};
}
