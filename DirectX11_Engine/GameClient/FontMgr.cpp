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
}

void FontMgr::Init()
{
	// 1. 팩토리 생성 확인
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		MessageBox(nullptr, L"FW1 Font Factory 생성 실패!", L"Font Error", MB_OK | MB_ICONERROR);
		return;
	}

	// 경로 설정 (상대 경로 확인 필수)
	wstring path = WCONTENT_PATH + L"Font\\_bitmap_font____romulus_by_pix3m-d6aokem.ttf";

	// 2. 파일 존재 여부 확인
	std::ifstream fontFile(path, std::ios::binary | std::ios::ate);
	if (!fontFile)
	{
		// Release 모드에서 경로 문제를 찾기 위해 시도한 전체 경로를 메시지에 담습니다.
		wstring errMsg = L"폰트 파일을 찾을 수 없습니다.\n경로: " + path;
		MessageBox(nullptr, errMsg.c_str(), L"File Not Found", MB_OK | MB_ICONWARNING);
		return;
	}

	// 파일 읽기 및 메모리 등록
	std::streamsize size = fontFile.tellg();
	fontFile.seekg(0, std::ios::beg);
	m_fontBuffer.resize((size_t)size);
	if (fontFile.read(reinterpret_cast<char*>(m_fontBuffer.data()), size))
	{
		DWORD fonts = 0;
		m_hFontResource = AddFontMemResourceEx(m_fontBuffer.data(), (DWORD)size, 0, &fonts);

		if (nullptr == m_hFontResource)
		{
			MessageBox(nullptr, L"AddFontMemResourceEx 실패!", L"Memory Font Error", MB_OK);
		}
	}

	// 3. 최종 폰트 래퍼 생성 확인
	// 폰트 메타데이터 분석 결과 "Romulus"가 맞으므로 이 이름은 그대로 사용합니다.
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

