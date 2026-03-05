#include "pch.h"
#include "Engine.h"
#include "Device.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "TaskMgr.h"
#include "EditorMgr.h"

Engine::Engine()
    : // 명시되어 있지 않지만, 상속받은 부모의 생성자가 숨어있음
	m_hInst(nullptr)
    , m_hWnd(nullptr)
    , m_Resolution{}
	, m_EditorMode(true)
{
}

Engine::~Engine()
{
}

int Engine::Progress() 
{
	// DeltaTime 계산
	TimeMgr::GetInst()->Tick();

	// Key 상태 계산
	KeyMgr::GetInst()->Tick();

	// Level 업데이트
	LevelMgr::GetInst()->Progress();

	// 렌더링
	RenderMgr::GetInst()->Progress();

	// imgui Editor 관리
	if (m_EditorMode)
		EditorMgr::GetInst()->Progress();

	/******************************************************************************
	* 렌더링은 CPU를 이용하거나, GPU를 이용해야 합니다.
	*   => 알맞은 API 함수를 활용
	*	=> CPU를 쓰고싶다면 Windows에서 제공하는 GDI(Graphics Device Interface)함수 이용
	*	=> GPU를 쓰고싶다면 Directx API(함수)를 이용해서 GPU를 컨트롤하고 명령해야 합니다.
	*
	* CPU는 싱글 스레드(단일 코어 성능)에 최적
	*	=> 난제를 풀기에 특화
	* GPU는 병렬처리에 최적 (성능이 CPU보다 낮지만, 코어 숫자가 많음)
	*	=> 노가다에 특화
	*
	* 컴퓨터 화면은 여러개의 픽셀로 나뉘어져 있음 (1차원 배열 형태로)
	* 1920x1080 모니터는 2,073,600의 픽셀로 이루어져 있습니다.
	* 색상정보는 4바이트에 RGB+A 정보를 담고 있음
	* RGB 1바이트당 0~255 범위에서 숫자로 표현
	******************************************************************************/

	// 렌더타겟에 그려진 그림을, 윈도우 비트맵으로 복사
	Device::GetInst()->Present();

	// 다음 프레임에 적용될 작업 관리
	TaskMgr::GetInst()->Progress();

	return S_OK;
}
