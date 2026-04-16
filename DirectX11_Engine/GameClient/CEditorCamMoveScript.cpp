#include "pch.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"

#include "CEditorCamMoveScript.h"
#include "CTransform.h"
#include "CCamera.h"

CEditorCamMoveScript::CEditorCamMoveScript()
	: CScript(-1)	// Engine 용 Script이기 때문에 따로 처리
	, m_isMoving(false)
	
	, m_OriginPos{}
	, m_OriginRot{}
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
	// 에디터 카메라 초기 위치를 아래 수치로 고정 등록
	m_OriginPos = Vec3(0.f, 900.f, -350.f);
	m_OriginRot = Vec3(0.f, 0.f, 0.f);
	Transform()->SetRelativePos(m_OriginPos);
	Transform()->SetRelativeRot(m_OriginRot);
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
		// NOTE(26-04-01):
		// Editor 편집 시, 마우스 움직임이 불편하여
		// 임시 비활성화
		//MouseCamMove();
	}
	// 직교 투영
	else if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
		OrthoCamMove();

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

void CEditorCamMoveScript::OrthoCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();

    // Use WASD for editor ortho camera movement
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
	Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
}

void CEditorCamMoveScript::PrespecCamMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

    // Use WASD for editor perspective camera movement
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
