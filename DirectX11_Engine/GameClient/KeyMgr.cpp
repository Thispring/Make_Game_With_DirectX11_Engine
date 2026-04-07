#include "pch.h"
#include "KeyMgr.h"
#include "Engine.h"

// KEY와 대응되는 인덱스 설정
UINT g_KeyIndex[(UINT)KEY::KEY_END] =
{
	'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	
	VK_LBUTTON,
	VK_RBUTTON,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_RETURN,
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_SPACE,
	VK_ESCAPE,
	VK_INSERT,  // INS
	VK_HOME,    // HOME
	VK_PRIOR,   // PAGE_UP
	VK_DELETE,  // DEL
	VK_END,     // END
	VK_NEXT,    // PAGE_DOWN

	// 상단 숫자 키
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

	// 숫자 패드 키
	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, 
	VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,

	VK_DIVIDE,   // NUMPAD_DIV
	VK_MULTIPLY, // NUMPAD_MUL
	VK_SUBTRACT, // NUMPAD_SUB

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,


	VK_OEM_3,    // ~
	VK_OEM_4,    // [
	VK_OEM_6,    // ]
	VK_OEM_MINUS, // -
	VK_OEM_PLUS,  // =
};

KeyMgr::KeyMgr()
	: m_isKeyPressed(false)
	, m_Active(false)
	, m_MouseWheelDelta(0)
{

}

KeyMgr::~KeyMgr()
{

}

void KeyMgr::Init()
{
	// 키 등록, std 벡터 사용
	m_vecKeys.resize((UINT)KEY::KEY_END);
}

void KeyMgr::Tick()
{
	// GetFocus : 현재 포커싱 중인 윈도우 핸들 반환
	if (GetFocus() == Engine::GetInst()->GetMainWndHwnd() && m_Active)
	{
		for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
		{
			// 지금 눌려있는지
			if (GetAsyncKeyState(g_KeyIndex[i]))
			{

				// 이전에도 눌려있었다.
				if (m_vecKeys[i].isPressed)
				{
					m_vecKeys[i].State = PRESSED;
				}

				// 이전까지는 눌린적이 없었다.
				else
				{
					m_vecKeys[i].State = TAP;
				}

				m_vecKeys[i].isPressed = true;
			}

			// 지금 안눌려있다.
			else
			{

				// 이전에는 눌려있었다.
				if (m_vecKeys[i].isPressed)
				{
					m_vecKeys[i].State = RELEASED;
				}
				else
				{
					m_vecKeys[i].State = NONE;
				}

				m_vecKeys[i].isPressed = false;
			}

		}


	}
	else
	{
		for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
		{
			// EditorUI 쪽에 포커싱 있을때 키가 눌리면, 누적된 입력으로 적용되는 문제 해결
			GetAsyncKeyState(g_KeyIndex[i]);

			if (m_vecKeys[i].State == KEY_STATE::TAP
				|| m_vecKeys[i].State == KEY_STATE::PRESSED)
			{
				m_vecKeys[i].State = KEY_STATE::RELEASED;
			}
			else
				m_vecKeys[i].State = KEY_STATE::NONE;

			m_vecKeys[i].isPressed = false;
		}

	}

	// 마우스 좌표 계산
	POINT pt = {};
	GetCursorPos(&pt); 	// 윈도우 기준 마우스 좌표
	ScreenToClient(Engine::GetInst()->GetMainWndHwnd(), &pt); 	// 클라이언트(윈도우 핸들) 기준 마우스 좌표

	m_MousePrevPos = m_MousePos;
	m_MousePos = Vec2((float)pt.x, (float)pt.y);
	// 두 연산을 통해 이전 프레임 마우스 좌표와 현재 프레임 마우스 좌표의
	// 이동 변화량을 알 수 있다.

	// 마우스 진행 방향
	m_MouseDir = m_MousePos - m_MousePrevPos;
}
