#include "pch.h"

#include "TimeMgr.h"
#include "LevelMgr.h"
#include "FontMgr.h"

#include "Engine.h"
#include "Device.h"

TimeMgr::TimeMgr()
	: m_Frequency{}
	, m_Prev{}
	, m_Current{}
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_FPS(0)
	, m_TargetFPS(0)
	, m_TargetFrameTime(0.0)

	, m_TotalPlayTime(0.f)
	, m_GoalTime(0.f)
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

	// 목표 시간 초기화 
	m_GoalTime = 600.f;
	m_TotalPlayTime = 0.f;

	// 1초 주기 갱신 전에도 Render에서 MeasureText가 유효한 문자열을 받도록 초기화
	m_strGoalTime  = L"10:00";
	m_strFPS       = L"";
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
	
	// 디버깅 중 중단점으로 인해 프레임 간격이 비정상적으로 커지는 경우를 무시
	// (요청대로 m_DeltaTime이 1 이상이면 0으로 고정)
	if (m_DeltaTime >= 1.0f)
	{
		m_DeltaTime = 0.1f;
	}

	m_Prev = m_Current;

	// Engine용 Time — 레벨 상태 무관하게 항상 누적
	g_Global.EngineDT = m_DeltaTime;
	g_Global.EngineTime += m_DeltaTime;

	// 레벨 상태에 따라 Content용 Time 분기
	const bool isPlaying = (LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState());
	if (!isPlaying)
	{
		g_Global.DeltaTime = m_DeltaTime = 0.f;
		g_Global.Time = 0.f;
	}
	else
	{
		// PLAY 상태에서만 플레이 시간·목표 시간 갱신
		m_TotalPlayTime += m_DeltaTime;
		m_GoalTime -= m_DeltaTime;	
		if (m_GoalTime < 0.f) m_GoalTime = 0.f;

		g_Global.DeltaTime = m_DeltaTime;
		g_Global.Time += m_DeltaTime;
	}

	// 1초 주기 FPS 카운터 (Engine 실시간 기준으로 항상 동작)
	m_Time += g_Global.EngineDT;
	if (1.f < m_Time)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_FPS);
		SetWindowText(Engine::GetInst()->GetMainWndHwnd(), buff);
		m_strFPS = buff;


		// %.nf: 소수점 아래 n자리까지 표시
		// 반올림 문제가 있을 수 있으므로 정수로 형변환하여 출력
		// 전체 초를 정수로 자른 뒤 분/초로 분리
		int totalSecs = (int)m_GoalTime;
		int minutes = totalSecs / 60;
		int seconds = totalSecs % 60;

		wchar_t goalTimeBuff[255] = {};
		// %02d: 한 자리 수일 때 앞에 0 패딩 (예: 9 → "09")
		swprintf_s(goalTimeBuff, 255, L"%d:%02d", minutes, seconds);
		m_strGoalTime = goalTimeBuff;

		m_FPS = 0;
		m_Time -= 1.f;
	}
}

void TimeMgr::Render()
{
	// FPS
	//FontMgr::GetInst()->DrawFont(m_strFPS.c_str(), 10, 30, 24, FONT_RGBA(200, 20, 20, 255));

	// 등록된 이후 호출하도록 변경
	if (LevelMgr::GetInst()->GetCurLevel() != nullptr)
	{
		// Level이 L"Level\\Normal_Stage_0.lv" 이고, PLAY 상태일때만 실행
		if (LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\Normal_Stage_0.lv"
			&& LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
		{
			PlayTimeRender();
		}
	}
	
}

void TimeMgr::PlayTimeRender()
{
	//================================================================================
	// 아웃라인 포함 — 흰 텍스트 + 검정 아웃라인, 두께 1.5px
	//	FontMgr::GetInst()->DrawFontOutline(m_strPlayTime.c_str(), 10.f, 50.f, 24.f,
	//	FONT_RGBA(255, 255, 255, 255),   // 본체: 흰색
	//	FONT_RGBA(0, 0, 0, 255),   // 아웃라인: 검정
	//	1.5f);
	//================================================================================

	// 현재 렌더 해상도 기준 X축 중앙 정렬 (창모드·전체화면 자동 대응)
	Vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
	Vec2 vGoalTextSize = FontMgr::GetInst()->MeasureText(m_strGoalTime.c_str(), 96.f);

	// 빈 문자열이거나 MeasureText가 유효하지 않은 값을 반환한 경우 폴백
	float fGoalPosX = 0.f;
	if (vGoalTextSize.x > 0.f && vGoalTextSize.x < vRenderResol.x)
		fGoalPosX = (vRenderResol.x - vGoalTextSize.x) / 2.f;
	else
		fGoalPosX = vRenderResol.x / 2.f;

	FontMgr::GetInst()->DrawFontOutline(m_strGoalTime.c_str(), fGoalPosX, 150.f, 96.f,
		FONT_RGBA(255, 255, 255, 255),
		FONT_RGBA(0, 0, 0, 255),
		1.5f);
}

void TimeMgr::InitPlayTime()
{
	m_TotalPlayTime = 0.f;
}

void TimeMgr::InitGoalTime()
{
	m_GoalTime = 600.f;
}
