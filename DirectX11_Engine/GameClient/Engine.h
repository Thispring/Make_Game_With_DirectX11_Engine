#pragma once
/*******************************************************************
* 디자인 패턴:
* 프로그래밍을 할 때 발생하는 다양한 설계 패턴
* 문제를 해결하기 위해 어떤 방식을 사용할지 설계하는 것
* 
* 디자인 패턴 종류:
* [iterator 패턴] - 자료구조 컨테이너를 설계할때, 내부동작 원리는 감추고, 
* 저장된 데이터를 쉽게 접근할 수 있게 하는 iterator 클래스를 같이 제공하는 패턴
* 
* [싱글턴 패턴] - 자료형(클래스)으로 생성시킬 수 있는 객체의 개수를 
* 1개로 제한하는 디자인 패턴
*******************************************************************/

// Engine은 게임 프로그램의 최상위 관리자 역할을 합니다.
class Engine
	: public Singleton<Engine>	// 템플릿 T 자리에 자신 객체를 넣음
{
	SINGLE(Engine)
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Vec2		m_Resolution;
	bool		m_EditorMode;	// Engine을 Editor로 실행할지 여부
	bool		m_FullScreen;	// 전체화면(창모드 1920x1080) 여부

public:
	//=========
	// 멤버 함수
	//=========
	int Init(HINSTANCE _hInst, UINT _Width, UINT _Height, bool _EditorMode);
	int Progress();
	void ToggleFullScreen();


	//=========
	// Get, Set
	//=========
	HINSTANCE GetInstance() { return m_hInst; }
	HWND GetMainWndHwnd() { return m_hWnd; }
	Vec2 GetResolution() { return m_Resolution; }
	bool IsFullScreen() const { return m_FullScreen; }

};
