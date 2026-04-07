#pragma once
#include "CCamera.h"

class CinematicMgr
	: public Singleton<CinematicMgr>
{
	SINGLE(CinematicMgr)
private:
	Ptr<CCamera>	m_TargetCam;

	Vec3			m_OriginPos;
	Vec3			m_TargetPos;

	static constexpr float CAM_MOVE_SPEED = 500.f;		// 카메라 이동 속도 (단위/초)
	static constexpr float ARRIVE_DIST    = 5.f;		// 목표 도달로 판정하는 거리 임계값

public:
	//=========
	// 멤버 함수
	//=========
	void Init();		// 레벨 Clone/Begin 이후 호출 — 카메라 탐색 및 원점 저장
	void Progress();

	// 카메라를 m_TargetPos 방향으로 이동시킵니다.
	// 목표 위치에 도달하면 true를 반환합니다.
	bool CameraMove();

	void RegisterCamera(Ptr<CCamera> _Cam) { m_TargetCam = _Cam; };
};
