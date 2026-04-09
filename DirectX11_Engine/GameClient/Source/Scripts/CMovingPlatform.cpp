#include "pch.h"
#include "CMovingPlatform.h"

#include "TimeMgr.h"

#include "GameObject.h"

CMovingPlatform::CMovingPlatform()
	: CScript(SCRIPT_TYPE::MOVINGPLATFORM)
	, m_OriginPos{}
	, m_TargetPos{}

	, m_OriginRot{}
	, m_TargetRot{}

	, m_Time(0.f)
	, m_Speed(0.f)
	, m_Amplitude(0.f)
	, m_RotAngle(0.f)

	, m_BobTime(0.f)
	, m_BobSpeed(0.f)
	, m_BobAmplitude(0.f)
	, m_BobRotAngle(0.f)
	, m_Dir(1)

	, m_PrevPos{}
	, m_Delta{}
{
}

CMovingPlatform::~CMovingPlatform()
{
}


void CMovingPlatform::PlatformMove()
{
	// DT를 누적 → sinf 입력값이 매 프레임 증가하여 올바르게 진동
	m_BobTime += DT;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	// 목표 위치의 x축이 설정되어 있다면 좌우 이동
	// 목표 위치의 y축이 설정되어 있다면 상하 이동
	
	//====================================================
	// abs(x) > FLT_EPSILON => "0이 아니다"(오차보다 크다)
	// abs(x) <= FLT_EPSILON => "0이다"(오차보다 작거나 같다)
	//====================================================
	// TargetPos x축이 0이 아니면 x축 이동 실행
	// 원점 y축과 목표 y축의 차이가 별로 없어야함
	if (abs(m_TargetPos.x) > FLT_EPSILON &&
		abs(m_TargetPos.y - m_OriginPos.y) <= FLT_EPSILON)
	{
		// m_Dir에 따라 목적지를 결정 (저장값은 변경하지 않음)
		float dest = (m_Dir > 0) ? m_TargetPos.x : m_OriginPos.x;
		float dx = dest - vPos.x;

		if (abs(dx) <= FLT_EPSILON)
		{
			vPos.x = dest;
			m_Dir *= -1;    // 도착 → 방향 반전
			GetOwner()->Transform()->SetRelativePos(vPos);
			return;
		}

		float sign = (dx > 0.f) ? 1.f : -1.f;
		float step = m_BobSpeed * DT;

		if (step >= abs(dx))
		{
			vPos.x = dest;
			m_Dir *= -1;    // 도착 → 방향 반전
		}
		else
		{
			vPos.x += sign * step;
		}

		GetOwner()->Transform()->SetRelativePos(vPos);
		return;
	}

	// y축 이동
	if (abs(m_TargetPos.y) > FLT_EPSILON &&
		abs(m_TargetPos.x - m_OriginPos.x) <= FLT_EPSILON)
	{
		// m_Dir에 따라 목적지를 결정 (저장값은 변경하지 않음)
		float dest = (m_Dir > 0) ? m_TargetPos.y : m_OriginPos.y;
		float dy = dest - vPos.y;

		if (abs(dy) <= FLT_EPSILON)
		{
			vPos.y = dest;
			m_Dir *= -1;    // 도착 → 방향 반전
			GetOwner()->Transform()->SetRelativePos(vPos);
			return;
		}

		float sign = (dy > 0.f) ? 1.f : -1.f;
		float step = m_BobSpeed * DT;

		if (step >= abs(dy))
		{
			vPos.y = dest;
			m_Dir *= -1;    // 도착 → 방향 반전
		}
		else
		{
			vPos.y += sign * step;
		}

		GetOwner()->Transform()->SetRelativePos(vPos);
		return;
	}

}

void CMovingPlatform::PlatformRotation()
{
}

void CMovingPlatform::Init()
{
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_OriginPos, L"OriginPos", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_TargetPos, L"TargetPos", true, 0.f);

	// 회전은 도 <-> 라디안 변환 주의
	AddScriptParam(SCRIPT_PARAM::VEC3_ROT, &m_OriginRot, L"OriginRot", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::VEC3_ROT, &m_TargetRot, L"TargetRot", true, 0.f);


	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Time, L"Time", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Amplitude, L"Amplitude", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotAngle, L"RotAngle", true, 0.f);
}

void CMovingPlatform::Begin()
{
	GetOwner()->Transform()->SetRelativePos(m_OriginPos);
	GetOwner()->Transform()->SetRelativeRot(m_OriginRot);

	m_BobTime = m_Time;
	m_BobSpeed = m_Speed;
	m_BobAmplitude = m_Amplitude;
	m_BobRotAngle = m_RotAngle;

	// delta 계산 초기화
	m_PrevPos = m_OriginPos;
	m_Delta = Vec3(0.f, 0.f, 0.f);
}

void CMovingPlatform::Tick()
{
	// 이동 전 위치 기록
	m_PrevPos = GetOwner()->Transform()->GetRelativePos();

	// Pos 값이 0이 아닌 값이 들어와야 실행
		// z축은 고정되어야 하므로 조건 검사 X
	if (abs(m_TargetPos.x) <= FLT_EPSILON &&
		abs(m_TargetPos.y) <= FLT_EPSILON)
	{
		m_Delta = Vec3(0.f, 0.f, 0.f);
		return;
	}
	else
		PlatformMove();

	if (abs(m_TargetRot.x) <= FLT_EPSILON ||
		abs(m_TargetRot.y) <= FLT_EPSILON)
	{
	}
	else
		PlatformRotation();

	// 이동 후 위치와 비교하여 delta 계산
	Vec3 vCurPos = GetOwner()->Transform()->GetRelativePos();
	m_Delta = vCurPos - m_PrevPos;

}

void CMovingPlatform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_OriginPos, sizeof(Vec3), 1, _File);
	fwrite(&m_TargetPos, sizeof(Vec3), 1, _File);

	fwrite(&m_OriginRot, sizeof(Vec3), 1, _File);
	fwrite(&m_TargetRot, sizeof(Vec3), 1, _File);

	fwrite(&m_Time, sizeof(float), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
	fwrite(&m_Amplitude, sizeof(float), 1, _File);
	fwrite(&m_RotAngle, sizeof(float), 1, _File);
}

void CMovingPlatform::LoadFromLevelFile(FILE* _File)
{
	fread(&m_OriginPos, sizeof(Vec3), 1, _File);
	fread(&m_TargetPos, sizeof(Vec3), 1, _File);

	fread(&m_OriginRot, sizeof(Vec3), 1, _File);
	fread(&m_TargetRot, sizeof(Vec3), 1, _File);
	
	fread(&m_Time, sizeof(float), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_Amplitude, sizeof(float), 1, _File);
	fread(&m_RotAngle, sizeof(float), 1, _File);
}
