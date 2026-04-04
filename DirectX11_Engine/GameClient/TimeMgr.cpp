#include "pch.h"

#include "TimeMgr.h"
#include "LevelMgr.h"
#include "FontMgr.h"

#include "Engine.h"

TimeMgr::TimeMgr()
	: m_Frequency{}
	, m_Prev{}
	, m_Current{}
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_FPS(0)
	, m_TargetFPS(0)
	, m_TargetFrameTime(0.0)
{

}

TimeMgr::~TimeMgr()
{

}

void TimeMgr::Init()
{
	// 1초동안 가능한 카운팅 횟수
	QueryPerformanceFrequency(&m_Frequency);
	
	// 초기 카운트
	QueryPerformanceCounter(&m_Current);
	QueryPerformanceCounter(&m_Prev);
}

void TimeMgr::SetTargetFPS(UINT _FPS)
{
	m_TargetFPS = _FPS;
	if (_FPS > 0)
		m_TargetFrameTime = 1.0 / (double)_FPS;
	else
		m_TargetFrameTime = 0.0;
}

bool TimeMgr::WaitForTargetFrame()
{
	// 프레임 제한이 설정되지 않았으면 바로 통과
	if (m_TargetFPS == 0)
		return false;

	// 현재까지 경과한 시간을 계산
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsed = (double)(now.QuadPart - m_Prev.QuadPart) / (double)m_Frequency.QuadPart;

	// 목표 프레임 시간에 도달하지 못했으면 스킵
	if (elapsed < m_TargetFrameTime)
		return true;

	return false;
}

void TimeMgr::Tick()
{
	++m_FPS;

	QueryPerformanceCounter(&m_Current);
	
	/*********************************************************
	* 1프레임이 돌아온 다시 돌아온 시간을 계산
	* _LARGE_INTEGER에 들어있는 변수들은 정수이므로,
	* 나눗셈을 한다면 나누어 떨어질때 까지 연산하므로, float로 형변환
	*********************************************************/
	m_DeltaTime = (float)(m_Current.QuadPart - m_Prev.QuadPart) / (float)m_Frequency.QuadPart;

	// Prev 카운팅을 다시 현재 카운팅으로 맞추기
	m_Prev = m_Current;

	// 누적 시간 계산
	m_Time += m_DeltaTime;

	// 1초마다 수행할 동작은 아래에서 정의
	if (1.f < m_Time)
	{
		// Windows 창에 몇 FPS가 나오는지 표시
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_FPS);
		SetWindowText(Engine::GetInst()->GetMainWndHwnd(), buff);
		m_strFPS = buff;	// FontMgr 출력용

		m_FPS = 0;
		m_Time -= 1.f;
	}

	// Engine용 Time 계산
	g_Global.EngineDT = m_DeltaTime;
	g_Global.EngineTime += m_DeltaTime;

	// Level이 Pause or Stop이라면
	if (LEVEL_STATE::PLAY != LevelMgr::GetInst()->GetLevelState())
	{
		// Content 용 Time 0으로 초기화
		g_Global.DeltaTime = m_DeltaTime = 0.f;
		g_Global.Time = 0.f;
	}
	// Level이 Play 라면
	else
	{
		// Content 용 Time 연산
		g_Global.DeltaTime = m_DeltaTime;
		g_Global.Time += m_DeltaTime;
	}
}

void TimeMgr::Render()
{
	FontMgr::GetInst()->DrawFont(m_strFPS.c_str(), 10, 30, 24, FONT_RGBA(200, 20, 20, 255));
}
