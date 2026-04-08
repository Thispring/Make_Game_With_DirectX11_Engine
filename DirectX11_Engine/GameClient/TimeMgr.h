#pragma once

// 게임에서 흐른 시간 및 프레임 당 간격 시간을 관리합니다.
class TimeMgr
	: public Singleton<TimeMgr>
{
	SINGLE(TimeMgr);
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_Prev;
	LARGE_INTEGER	m_Current;

	UINT			m_FPS;

	float			m_DeltaTime;	// 1 프레임 간격 시간
	float			m_Time;			// 게임이 켜진 이후로 흐른 시간(누적)

	float			m_TotalPlayTime;	// PLAY 상태에서만 누적되는 총 플레이 시간 
	float			m_GoalTime;			// 게임 콘텐츠 용 목표 시간

	// 프레임 고정
	UINT			m_TargetFPS;		// 목표 FPS (0이면 제한 없음)
	double			m_TargetFrameTime;	// 1프레임 목표 시간 (초)

	wstring			m_strFPS;
	wstring			m_strPlayTime;
	wstring			m_strGoalTime;

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Tick();
	void Render();
	void PlayTimeRender();
	void InitPlayTime();
	void InitGoalTime();

	/// @brief 프레임이 목표 시간에 도달할 때까지 대기합니다.
	/// @return 목표 시간 미달이면 true(스킵), 도달했으면 false
	bool WaitForTargetFrame();

	//=========
	// Get, Set
	//=========
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return g_Global.Time; }

	// Engine DT, Time 추가
	float GetEngineDT() { return g_Global.EngineDT; }
	float GetEngineTime() { return g_Global.EngineTime; }

	float GetTotalPlayTime() { return m_TotalPlayTime; }

	/// @brief 목표 FPS를 설정합니다. 0을 넣으면 제한 없음.
	void SetTargetFPS(UINT _FPS);
};

// 자주 사용하는 함수 재정의
#define DT		TimeMgr::GetInst()->GetDeltaTime()
#define TIME	TimeMgr::GetInst()->GetTime()
// Engine Time 매크로
#define E_DT	TimeMgr::GetInst()->GetEngineDT()
#define E_Time	TimeMgr::GetInst()->GetEngineTime()
