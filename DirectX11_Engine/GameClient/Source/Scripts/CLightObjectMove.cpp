#include "pch.h"
#include "CLightObjectMove.h"
#include "GameObject.h"
#include "KeyMgr.h"
#include "RenderMgr.h"
#include "Engine.h"

CLightObjectMove::CLightObjectMove()
	: CScript(SCRIPT_TYPE::LIGHTOBJECTMOVE)
{
}

CLightObjectMove::~CLightObjectMove()
{
}

void CLightObjectMove::RotateTowardsMouse()
{
	// 클라이언트(윈도우) 기준 마우스 픽셀 좌표
	Vec2 mouse = KeyMgr::GetInst()->GetMousePos();

	// 화면 중심을 기준으로 방향 계산 (원하면 다른 기준점으로 변경 가능)
	Vec2 res = Engine::GetInst()->GetResolution();
	float cx = res.x * 0.5f;
	float cy = res.y * 0.5f;

	float dx = mouse.x - cx;
	float dy = -(mouse.y - cy); // 화면 Y는 아래가 + 이므로 위를 +로 만들기 위해 반전

	// 너무 가까우면 회전하지 않음
	if (fabsf(dx) < 1e-6f && fabsf(dy) < 1e-6f)
		return;

	// atan2(y, x) -> 라디안
	float desiredAngle = atan2f(dy, dx);

	// 현재 월드 회전 가져와 Z에 적용
	Vec3 curWorldRot = GetOwner()->Transform()->GetWorldRot();
	curWorldRot.z = desiredAngle;
	GetOwner()->Transform()->SetWorldRot(curWorldRot);
}


void CLightObjectMove::Begin()
{

}

void CLightObjectMove::Tick()
{
	RotateTowardsMouse();
}

void CLightObjectMove::SaveToLevelFile(FILE* _File)
{
}

void CLightObjectMove::LoadFromLevelFile(FILE* _File)
{
}
