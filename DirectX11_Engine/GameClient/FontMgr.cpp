#include "pch.h"
#include "FontMgr.h"

#include "Device.h"
#include "PathMgr.h"
#include "LevelMgr.h"
#include "ScoreMgr.h"

FontMgr::FontMgr()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
	, m_hFontResource(nullptr)
{
}

FontMgr::~FontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();

	// 메모리 폰트 해제
	if (m_hFontResource && !m_fontBuffer.empty())
	{
		RemoveFontMemResourceEx(m_hFontResource);
		m_hFontResource = nullptr;
		m_fontBuffer.clear();
	}

	// [추가] 임시 등록했던 시스템 폰트 리소스 해제
	wstring path = WCONTENT_PATH + L"Font\\_bitmap_font____romulus_by_pix3m-d6aokem.ttf";
	RemoveFontResourceEx(path.c_str(), FR_PRIVATE, 0);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
}

void FontMgr::Init()
{
	// 1. FW1 팩토리 생성
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		MessageBox(nullptr, L"FW1 Font Factory 생성 실패!", L"Font Error", MB_OK | MB_ICONERROR);
		return;
	}

	// 경로 설정
	wstring path = WCONTENT_PATH + L"Font\\_bitmap_font____romulus_by_pix3m-d6aokem.ttf";

	// [패치 1] 시스템 임시 리소스로 직접 등록 (AddFontResourceEx)
	// FR_PRIVATE를 사용하면 현재 프로세스가 실행 중일 때만 유효하며, 설치 없이도 시스템 폰트처럼 인식률이 높아집니다.
	int resCount = AddFontResourceEx(path.c_str(), FR_PRIVATE, 0);

	if (resCount > 0)
	{
		// [패치 2] OS에 폰트 테이블이 변경되었음을 알림 (브로드캐스트)
		// 이 과정을 통해 FW1Wrapper가 폰트를 찾을 확률이 비약적으로 상승합니다.
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	}
	else
	{
		// 파일은 있지만 등록에 실패한 경우 (권한 문제 등)를 위한 백업 메시지
#ifndef _DEBUG
// 릴리즈 빌드에서만 상세 경로 노출
		wstring errMsg = L"폰트 리소스 등록 실패(시스템 예약 오류).\n경로: " + path;
		MessageBox(nullptr, errMsg.c_str(), L"Font Registration Fail", MB_OK | MB_ICONWARNING);
#endif
	}

	// [패치 3] 메모리 로드 로직 (기존 유지하되 안전장치 추가)
	std::ifstream fontFile(path, std::ios::binary | std::ios::ate);
	if (fontFile)
	{
		std::streamsize size = fontFile.tellg();
		fontFile.seekg(0, std::ios::beg);
		m_fontBuffer.resize((size_t)size);

		if (fontFile.read(reinterpret_cast<char*>(m_fontBuffer.data()), size))
		{
			DWORD fonts = 0;
			m_hFontResource = AddFontMemResourceEx(m_fontBuffer.data(), (DWORD)size, 0, &fonts);
		}
		fontFile.close();
	}

	// 2. 폰트 래퍼 생성
	// 폰트 등록 후 OS가 인덱싱할 시간을 아주 잠깐 벌어주는 것이 안전합니다.
	Sleep(10);

	// L"Romulus" 이름이 정확한지 다시 한번 확인 필수
	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Romulus", &m_FontWrapper)))
	{
		MessageBox(nullptr, L"Font Wrapper 생성 실패! (Romulus 글꼴 인식 불가)", L"Font Error", MB_OK | MB_ICONERROR);
	}
}

void FontMgr::PrintEnding()
{
	// Test Font 렌더
	// 등록된 이후 호출하도록 변경
	if (LevelMgr::GetInst()->GetCurLevel() != nullptr)
	{
		// Level이 L"Level\\Ending.lv" 이고, PLAY 상태일때만 실행
		if (LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\Ending.lv"
			&& LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
		{
			// 현재 렌더 해상도 기준 X축 중앙 정렬 (창모드·전체화면 자동 대응)
			Vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
			Vec2 vGoalTextSize = FontMgr::GetInst()->MeasureText(L"GameClear", 96.f);

			// 빈 문자열이거나 MeasureText가 유효하지 않은 값을 반환한 경우 폴백
			float fGoalPosX = 0.f;
			if (vGoalTextSize.x > 0.f && vGoalTextSize.x < vRenderResol.x)
				fGoalPosX = (vRenderResol.x - vGoalTextSize.x) / 2.f;
			else
				fGoalPosX = vRenderResol.x / 2.f;

			FontMgr::GetInst()->DrawFontOutline(L"Ending", fGoalPosX, 150, 96,
				FONT_RGBA(255, 255, 255, 255),
				FONT_RGBA(0, 0, 0, 255),
				1.5f);

			// 아래 최고 점수 출력
			ScoreMgr::GetInst()->RenderScore();
		}
	}
}

void FontMgr::PrintGameOver()
{
	// 등록된 이후 호출하도록 변경
	if (LevelMgr::GetInst()->GetCurLevel() != nullptr)
	{
		// Level이 L"Level\\GameOver.lv" 이고, PLAY 상태일때만 실행
		if (LevelMgr::GetInst()->GetCurLevel()->GetKey() == L"Level\\GameOver.lv"
			&& LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
		{
			// 현재 렌더 해상도 기준 X축 중앙 정렬 (창모드·전체화면 자동 대응)
			Vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
			Vec2 vGoalTextSize = FontMgr::GetInst()->MeasureText(L"GameOver", 96.f);

			// 빈 문자열이거나 MeasureText가 유효하지 않은 값을 반환한 경우 폴백
			float fGoalPosX = 0.f;
			if (vGoalTextSize.x > 0.f && vGoalTextSize.x < vRenderResol.x)
				fGoalPosX = (vRenderResol.x - vGoalTextSize.x) / 2.f;
			else
				fGoalPosX = vRenderResol.x / 2.f;

			FontMgr::GetInst()->DrawFontOutline(L"GameOver", fGoalPosX, 150, 96,
				FONT_RGBA(255, 255, 255, 255),
				FONT_RGBA(0, 0, 0, 255),
				1.5f);
		}
	}
}

void FontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_FontWrapper->DrawString(
		CONTEXT,
		_pStr,
		_fFontSize,
		_fPosX,
		_fPosY,
		_Color,
		FW1_RESTORESTATE
	);
}

Vec2 FontMgr::MeasureText(const wchar_t* _pStr, float _fFontSize)
{
	// 너비·높이 0짜리 레이아웃을 사용하면 텍스트 전체가 오버행(overhang)으로 잡혀
	// GetOverhangMetrics의 right/bottom = 실제 텍스트 너비/높이가 그대로 반환됩니다.
	// 반면 큰 레이아웃(4096×4096)을 쓰면 레이아웃 크기가 Right에 포함되어 오차가 발생합니다.
	FW1_RECTF layoutRect = { 0.f, 0.f, 0.f, 0.f };

	FW1_RECTF measured = m_FontWrapper->MeasureString(
		_pStr,
		nullptr,		// 기본 폰트(Romulus) 사용
		_fFontSize,
		&layoutRect,
		FW1_NOWORDWRAP
	);

	return Vec2(measured.Right - measured.Left, measured.Bottom - measured.Top);
}

void FontMgr::DrawFontOutline(const wchar_t* _pStr, float _fPosX, float _fPosY,
                               float _fFontSize, UINT _FillColor, UINT _OutlineColor,
                               float _fThickness)
{
	// 8방향 오프셋으로 아웃라인 먼저 렌더
	// → 나중에 덮어쓸 본체보다 먼저 그려야 아웃라인이 뒤에 깔림
	const float offsets[8][2] =
	{
		{-1.f, -1.f}, { 0.f, -1.f}, { 1.f, -1.f},
		{-1.f,  0.f},               { 1.f,  0.f},
		{-1.f,  1.f}, { 0.f,  1.f}, { 1.f,  1.f}
	};

	for (int i = 0; i < 8; ++i)
	{
		m_FontWrapper->DrawString(
			CONTEXT, _pStr, _fFontSize,
			_fPosX + offsets[i][0] * _fThickness,
			_fPosY + offsets[i][1] * _fThickness,
			_OutlineColor,
			FW1_RESTORESTATE);
	}

	// 본체 렌더 — 아웃라인 위에 덮어 씀
	m_FontWrapper->DrawString(
		CONTEXT, _pStr, _fFontSize,
		_fPosX, _fPosY,
		_FillColor,
		FW1_RESTORESTATE);
}

