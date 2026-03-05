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

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Tick();


	//=========
	// Get, Set
	//=========
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return g_Global.Time; }

	// Engine DT, Time 추가
	float GetEngineDT() { return g_Global.EngineDT; }
	float GetEngineTime() { return g_Global.EngineTime; }
};

// 자주 사용하는 함수 재정의
#define DT		TimeMgr::GetInst()->GetDeltaTime()
#define TIME	TimeMgr::GetInst()->GetTime()
// Engine Time 매크로
#define E_DT	TimeMgr::GetInst()->GetEngineDT()
#define E_Time	TimeMgr::GetInst()->GetEngineTime()
