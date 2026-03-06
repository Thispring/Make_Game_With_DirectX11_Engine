#pragma once

// 프로그램에서 사용 중인 KEY enum
enum class KEY
{
	Q, 
	W, 
	E, 
	R, 
	A, 
	S, 
	D, 
	F, 
	Z, 
	X, 
	C, 
	V,

	LBTN,
	RBTN,

	LEFT, 
	RIGHT, 
	UP, 
	DOWN,
	ENTER, 
	ALT, 
	CTRL, 
	LSHIFT, 
	RSHIFT, 
	SPACE, 
	ESC,
	HOME, 
	END,
	
	// 상단 숫자 키
	ALPHA0, ALPHA1, ALPHA2, ALPHA3, ALPHA4, ALPHA5, ALPHA6, ALPHA7, ALPHA8, ALPHA9,

	// 숫자 패드 키
	NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,


	F1, F2, F3, F4, F5,
	F6, F7, F8, F9,

	KEY_END	// KEY 개수가 총 몇 개인지를 확인하는 용도
};

// None  TAP Pressed  Released None
//  |     |  |			  |     |
//  V     V  V            V     V 
//  ____				 _______________
//	    \_______________/
//
// KEY 입력 상태 enum
enum KEY_STATE
{
	NONE, TAP, PRESSED, RELEASED
};

struct KeyInfo
{
	KEY_STATE	State;			// 키의 현재 상태
	bool		isPressed;		// 이전에 눌린적이 있는지 아닌지
};

// 프로그램의 키보드, 마우스 입력을 관리합니다.
class KeyMgr
	: public Singleton<KeyMgr>
{
	SINGLE(KeyMgr)
private:
	vector<KeyInfo> m_vecKeys;

	Vec2 m_MousePos;				// 이번 프레임 마우스 좌표
	Vec2 m_MousePrevPos;			// 이전 프레임 마우스 좌표
	Vec2 m_MouseDir;				// 마우스 진행 방향

	int m_MouseWheelDelta;			// 마우스 휠 정보

	bool m_isKeyPressed;				// Key 입력이 있었는지 여부

	bool m_Active;		// Key입력을 받을지 말지를 결정

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Tick();

	// 마우스 휠 이벤트를 받기 위한 함수
	void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		switch (msg) 
		{
		case WM_MOUSEWHEEL:
			/*****************************************************
			* 마우스 휠 이벤트가 들어오면
			* 이번 마우스 휠 정보를 이전 프레임 마우스 휠 정보로 대입하고
			* 새로 입력받은 정보를 갱신합니다.
			*****************************************************/
			m_MouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		}
	}


	//=========
	// Get, Set
	//=========
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeys[(UINT)_Key].State; }

	GET(Vec2, MousePos);
	GET(Vec2, MousePrevPos);
	GET(Vec2, MouseDir);

	GET(bool, isKeyPressed);

	GET(int, MouseWheelDelta);
	SET(int, MouseWheelDelta);

	bool IsActive() { return m_Active; }
	void SetActive(bool _Active) { m_Active = _Active; }

};

// 자주 사용되는 함수 재정의
#define KEY_CHECK(key, state) KeyMgr::GetInst()->GetKeyState(key) == state

#define KEY_TAP(key)		KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_PRESSED(key)	KEY_CHECK(key, KEY_STATE::PRESSED)
#define KEY_RELEASED(key)	KEY_CHECK(key, KEY_STATE::RELEASED)
#define KEY_NONE(key)		KEY_CHECK(key, KEY_STATE::NONE)
