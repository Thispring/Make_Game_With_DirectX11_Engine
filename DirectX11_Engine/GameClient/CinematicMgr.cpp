#include "pch.h"
#include "CinematicMgr.h"
#include "TimeMgr.h"
#include "GameObject.h"

CinematicMgr::CinematicMgr()
	: m_TargetCam(nullptr)
	, m_OriginPos{}
	, m_TargetPos{Vec3(-4600.f, 217.715f, -350.f)}
{

}

CinematicMgr::~CinematicMgr()
{

}

void CinematicMgr::Init()
{
	//// 현재 레벨에서 MainCamera 오브젝트를 탐색하여 등록
	//Ptr<GameObject> pCamObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera");
	//assert(pCamObj != nullptr && "CinematicMgr::Init — MainCamera not found in level");

	//RegisterCamera(pCamObj->Camera());

	assert(m_TargetCam != nullptr && "CinematicMgr::Init — MainCamera not found in level");
	// 카메라의 현재 위치를 연출 시작 원점으로 저장
	m_OriginPos = m_TargetCam->GetOwner()->Transform()->GetRelativePos();
}

void CinematicMgr::Progress()
{

}

bool CinematicMgr::CameraMove()
{
	Vec3 vCurPos = m_TargetCam->GetOwner()->Transform()->GetRelativePos();
	Vec3 vDir    = m_TargetPos - vCurPos;
	float fDist  = vDir.Length();

	// 목표 거리 임계값 이내에 들어오면 위치를 정확히 고정하고 완료 반환
	if (fDist <= ARRIVE_DIST)
	{
		m_TargetCam->GetOwner()->Transform()->SetRelativePos(m_TargetPos);
		return true;
	}

	vDir.Normalize();
	m_TargetCam->GetOwner()->Transform()->SetRelativePos(vCurPos + vDir * CAM_MOVE_SPEED * E_DT);

	return false;
}
