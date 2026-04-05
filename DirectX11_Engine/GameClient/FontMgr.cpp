#include "pch.h"
#include "FontMgr.h"
#include "Device.h"
#include "PathMgr.h"

FontMgr::FontMgr()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
{
}

FontMgr::~FontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();

	wstring path = WCONTENT_PATH + L"\\Fonts\\_bitmap_font____romulus_by_pix3m-d6aokem.ttf";
	RemoveFontResourceEx(path.c_str(), FR_PRIVATE, 0);
}

void FontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper)))
	{
		assert(nullptr);
	}
	
	wstring path = WCONTENT_PATH + L"\\Fonts\\_bitmap_font____romulus_by_pix3m-d6aokem.ttf";
	
	// 1. 폰트 파일을 시스템 리소스에 추가
	AddFontResourceEx(path.c_str(), FR_PRIVATE, 0);

	// 2. 이제 시스템은 이 폰트를 알고 있으므로, 폰트의 '실제 이름'으로 생성 가능합니다.
	// 예: 파일명은 MyCustomFont.ttf여도 내부 이름이 "NanumGothic"이라면 해당 이름을 사용해야 함.
	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Romulus", &m_FontWrapper)))
	{
		assert(nullptr);
	}

	// 3. 프로그램 종료 시 리소스 해제
	// RemoveFontResourceEx(L"./Fonts/MyCustomFont.ttf", FR_PRIVATE, 0);
}

void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_FontWrapper->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// Window X position
		_fPosY,				// Window Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
