#include "pch.h"
#include "CUIOverlayController.h"

#include "GameObject.h"

#include "GameMgr.h"
#include "LevelMgr.h"

CUIOverlayController::CUIOverlayController()
	: CScript(SCRIPT_TYPE::UIOVERLAYCONTROLLER)
{
}

CUIOverlayController::~CUIOverlayController()
{
}

void CUIOverlayController::Begin()
{
	m_vecUIObject = GetOwner()->GetChild();

	// CUICamMoveScript가 카메라를 플레이어 XY로 이동시키므로
	// 카메라 위치 == 플레이어 위치가 항상 성립
	//LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetRelativePos();
	Vec3 vCamPos = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetRelativePos();
	//Vec3 vCamPos = GameMgr::GetInst()->GetPlayer()->Transform()->GetRelativePos();

	// 각 UI 오브젝트의 카메라 기준 초기 화면 오프셋 저장
	m_vecUIOffset.resize(m_vecUIObject.size());
	for (int i = 0; i < (int)m_vecUIObject.size(); i++)
	{
		Vec3 vUIPos = m_vecUIObject[i]->Transform()->GetRelativePos();
		m_vecUIOffset[i] = Vec3(vUIPos.x - vCamPos.x, vUIPos.y - vCamPos.y, 0.f);
	}
}

void CUIOverlayController::Tick()
{
	Vec3 vCamPos = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetRelativePos();

	for (int i = 0; i < (int)m_vecUIObject.size(); i++)
	{
		// Z는 UI 오브젝트 자신의 값을 유지 (렌더링 깊이/순서 보존)
		float fZ = m_vecUIObject[i]->Transform()->GetRelativePos().z;

		m_vecUIObject[i]->Transform()->SetRelativePos(
			Vec3(vCamPos.x + m_vecUIOffset[i].x,
				 vCamPos.y + m_vecUIOffset[i].y,
				 fZ));
	}
}

void CUIOverlayController::SaveToLevelFile(FILE* _File)
{
}

void CUIOverlayController::LoadFromLevelFile(FILE* _File)
{
}

