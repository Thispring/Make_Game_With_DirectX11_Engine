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

void CCamMoveScript::Init()
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
	m_Target = LevelMgr::GetInst()->FindObjectByName(L"Player");

	// 현재 Level이 Normal_Stage_0.lv 라면 아래 고정 Position을 적용
	if (LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\Normal_Stage_0.lv")
	{
		GetOwner()->Transform()->SetRelativePos(Vec3(-4600.f, 800.f, -350.f));
	}
}

void CCamMoveScript::Tick()
{
	// Change Cam Move Mode
	if (KEY_PRESSED(KEY::NUMPAD_DIV))
	{
		// F1 키로 카메라 모드 스위칭
		if (m_MoveMode == CAM_MOVE_MODE::DEBUG)
		{
			m_MoveMode = CAM_MOVE_MODE::INGAME;
			return;
		}

		if (m_MoveMode == CAM_MOVE_MODE::INGAME)
		{
			m_MoveMode = CAM_MOVE_MODE::DEBUG;
			return;
		}
	}

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
		// NOTE(26-03-28):
		// 해당 조건문은, 특정 Level에서만 사용합니다.
		// 임시로 Level의 Key값 문자열을 비교하여, 사용하지 않은 Level에서 return 반환
		// ex)Normal_Stage_0
		if (LevelMgr::GetInst()->GetCurLevel()->GetKey() != L"Level\\Normal_Stage_0.lv")
			return;

		Vec3 vPos = Transform()->GetRelativePos();

		// Player의 Z축을 제외하고 가져옵니다.
		Vec3 playerPos = m_Target->Transform()->GetRelativePos();
		playerPos.z = vPos.z;

		GetOwner()->Transform()->SetRelativePos(playerPos);
	}


	// I KEY를 누르면 위치, 회전 상태 초기화
	// Resets the position and rotation when the I KEY is pressed.
	if (KEY_PRESSED(KEY::NUMPAD0))
		MoveOrigin();

	// F4 KEY를 누르면 카메라의 투영방식 스위칭
	// Switches the camera's projection method when the F4 KEY is pressed.
	if (KEY_TAP(KEY::NUMPAD_SUB))
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

    // Move using WASD instead of arrow keys
	if (KEY_PRESSED(KEY::NUMPAD8))
		vPos.y += DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD5))
		vPos.y -= DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD4))
		vPos.x -= DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD6))
		vPos.x += DT * 1600.f;

	if (KEY_PRESSED(KEY::NUMPAD7))
		vPos.z += E_DT * 500.f;
	if (KEY_PRESSED(KEY::NUMPAD9))
		vPos.z -= E_DT * 500.f;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
}

void CCamMoveScript::PrespecCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

    // Use WASD for in-game camera movement as well
	if (KEY_PRESSED(KEY::NUMPAD8))
		vPos.y += E_DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD5))
		vPos.y -= E_DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD4))
		vPos.x -= E_DT * 1600.f;
	if (KEY_PRESSED(KEY::NUMPAD6))
		vPos.x += E_DT * 1600.f;

	if (KEY_PRESSED(KEY::NUMPAD7))
		vPos.z += E_DT * 500.f;
	if (KEY_PRESSED(KEY::NUMPAD9))
		vPos.z -= E_DT * 500.f;

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
