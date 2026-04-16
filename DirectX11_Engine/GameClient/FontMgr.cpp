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
	// 캐시된 TextLayout을 먼저 해제 (명시적 순서 보장)
	if (m_CachedTextLayout)
	{
		m_CachedTextLayout.Reset();
		m_CachedFontPath.clear();
		m_CachedFamilyName.clear();
		m_CachedText.clear();
		m_CachedFontSize = 0.f;
		m_CachedMaxWidth = 0.f;
		m_CachedMaxHeight = 0.f;
	}

	if (nullptr != m_FontWrapper)
	{
		m_FontWrapper->Release();
		m_FontWrapper = nullptr;
	}

	if (nullptr != m_FW1Factory)
	{
		m_FW1Factory->Release();
		m_FW1Factory = nullptr;
	}

	// 메모리 폰트 해제
	if (m_hFontResource && !m_fontBuffer.empty())
	{
		RemoveFontMemResourceEx(m_hFontResource);
		m_hFontResource = nullptr;
		m_fontBuffer.clear();
	}
}

void FontMgr::CreateFontWrapper()
{
	if (nullptr != m_FW1Factory)
	{
		m_FW1Factory->Release();
		m_FW1Factory = nullptr;
	}

	if (nullptr != m_FontWrapper)
	{
		m_FontWrapper->Release();
		m_FontWrapper = nullptr;
	}

	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper)))
	{
		assert(nullptr);
	}
}

int FontMgr::FontCheck()
{
	IDWriteFactory* IDFactory = nullptr;
	m_FontWrapper->GetDWriteFactory(&IDFactory);

	if (!IDFactory)
	{
		assert(nullptr);
	}

	IDWriteFontCollection* FontCollection = nullptr;
	HRESULT Hr = IDFactory->GetSystemFontCollection(&FontCollection, TRUE);

	if (FAILED(Hr) || !FontCollection)
		assert(nullptr);

	UINT32 FamilyIndex = 0;
	BOOL bExists = FALSE;
	Hr = FontCollection->FindFamilyName(m_FontFamily.c_str(), &FamilyIndex, &bExists);

	FontCollection->Release();

	return bExists;
}

void FontMgr::Init(const wstring& _FontPath, const wstring& _FontFamily)
{
	if (!_FontPath.empty())
	{
		m_FontPath = CONTENT_PATH + _FontPath;
		m_FontFamily = _FontFamily;
	}

	CreateFontWrapper();
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
	if (m_FontFamily.empty())
	{
		m_FontWrapper->DrawString(
			CONTEXT,
			_pStr,
			_fFontSize,				// Font size
			_fPosX,					// Window X position
			_fPosY,					// Window Y position
			_Color,					// Text color, 0xAaBbGgRr
			FW1_RESTORESTATE		// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
	}
	else
	{
		// 단일 캐시된 TextLayout을 가져오거나 생성
		ComPtr<IDWriteTextLayout> pTextLayout = {};
		if (FAILED(GetOrCreateCachedTextLayout(
			m_FontPath
			, m_FontFamily
			, _pStr
			, _fFontSize
			, 1000.f
			, 200.f
			, pTextLayout.GetAddressOf())))
		{
			return;
		}

		m_FontWrapper->DrawTextLayout(
			CONTEXT,
			pTextLayout.Get(),
			_fPosX,					// Window X position
			_fPosY,					// Window Y position
			_Color,					// Text color, 0xAaBbGgRr);
			FW1_RESTORESTATE);
	}
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

// ===============
// TextLayout 생성
// ===============
HRESULT FontMgr::CreateTextLayoutFromFontFile(
	const std::wstring& InFontFilePath,
	const std::wstring& InFamilyName,
	const std::wstring& InText,
	float InFontSize,
	float InMaxWidth,
	float InMaxHeight,
	IDWriteTextLayout** OutTextLayout)
{
	if (!OutTextLayout)
	{
		return E_INVALIDARG;
	}

	*OutTextLayout = nullptr;

	if (InFontFilePath.empty() || InFamilyName.empty() || InText.empty())
	{
		return E_INVALIDARG;
	}


	namespace fs = std::filesystem;

	std::error_code ErrorCode;
	const bool bExists = fs::exists(InFontFilePath, ErrorCode);
	const bool bIsFile = fs::is_regular_file(InFontFilePath, ErrorCode);

	if (!bExists || !bIsFile)
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	ComPtr<IDWriteFactory5> DWriteFactory;
	HRESULT Hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(DWriteFactory.GetAddressOf()));
	if (FAILED(Hr) || !DWriteFactory)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	ComPtr<IDWriteFontSetBuilder1> FontSetBuilder;
	Hr = DWriteFactory->CreateFontSetBuilder(&FontSetBuilder);
	if (FAILED(Hr) || !FontSetBuilder)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	ComPtr<IDWriteFontFile> FontFile;
	Hr = DWriteFactory->CreateFontFileReference(InFontFilePath.c_str(), nullptr, &FontFile);
	if (FAILED(Hr) || !FontFile)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	Hr = FontSetBuilder->AddFontFile(FontFile.Get());
	if (FAILED(Hr))
	{
		return Hr;
	}

	ComPtr<IDWriteFontSet> FontSet;
	Hr = FontSetBuilder->CreateFontSet(&FontSet);
	if (FAILED(Hr) || !FontSet)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	ComPtr<IDWriteFontCollection1> FontCollection;
	Hr = DWriteFactory->CreateFontCollectionFromFontSet(FontSet.Get(), &FontCollection);
	if (FAILED(Hr) || !FontCollection)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	UINT32 FamilyIndex = 0;
	BOOL bExistsFamily = FALSE;
	Hr = FontCollection->FindFamilyName(InFamilyName.c_str(), &FamilyIndex, &bExistsFamily);
	if (FAILED(Hr))
	{
		return Hr;
	}

	if (!bExistsFamily)
	{
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	}

	ComPtr<IDWriteTextFormat> TextFormat;
	Hr = DWriteFactory->CreateTextFormat(
		InFamilyName.c_str(),
		FontCollection.Get(),
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		InFontSize,
		L"ko-KR",
		&TextFormat);
	if (FAILED(Hr) || !TextFormat)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	Hr = TextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	if (FAILED(Hr))
	{
		return Hr;
	}

	Hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(Hr))
	{
		return Hr;
	}

	Hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (FAILED(Hr))
	{
		return Hr;
	}

	ComPtr<IDWriteTextLayout> TextLayout;
	Hr = DWriteFactory->CreateTextLayout(
		InText.c_str(),
		static_cast<UINT32>(InText.size()),
		TextFormat.Get(),
		InMaxWidth,
		InMaxHeight,
		&TextLayout);
	if (FAILED(Hr) || !TextLayout)
	{
		return FAILED(Hr) ? Hr : E_FAIL;
	}

	*OutTextLayout = TextLayout.Detach();
	return S_OK;
}

// 단일 캐시를 가져오거나 새로 생성해 캐시에 저장하는 헬퍼
HRESULT FontMgr::GetOrCreateCachedTextLayout(const std::wstring& InFontFilePath,
	const std::wstring& InFamilyName,
	const std::wstring& InText,
	float InFontSize,
	float InMaxWidth,
	float InMaxHeight,
	IDWriteTextLayout** OutTextLayout)
{
	if (!OutTextLayout)
		return E_INVALIDARG;

	*OutTextLayout = nullptr;

	// 캐시가 있고 파라미터가 동일하면 재사용
	if (m_CachedTextLayout &&
		m_CachedFontPath == InFontFilePath &&
		m_CachedFamilyName == InFamilyName &&
		m_CachedText == InText &&
		m_CachedFontSize == InFontSize &&
		m_CachedMaxWidth == InMaxWidth &&
		m_CachedMaxHeight == InMaxHeight)
	{
		return m_CachedTextLayout.CopyTo(OutTextLayout);
	}

	// 없으면 생성 후 캐시에 저장 (기존 것은 ComPtr가 자동으로 Release 처리)
	ComPtr<IDWriteTextLayout> newLayout;
	HRESULT hr = CreateTextLayoutFromFontFile(InFontFilePath, InFamilyName, InText, InFontSize, InMaxWidth, InMaxHeight, newLayout.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	// 캐시에 저장할 파라미터 복사
	m_CachedTextLayout = newLayout;
	m_CachedFontPath = InFontFilePath;
	m_CachedFamilyName = InFamilyName;
	m_CachedText = InText;
	m_CachedFontSize = InFontSize;
	m_CachedMaxWidth = InMaxWidth;
	m_CachedMaxHeight = InMaxHeight;

	return m_CachedTextLayout.CopyTo(OutTextLayout);
}

// ===============================================================================================


void FontMgr::DrawFontOutline(const wchar_t* _pStr, float _fPosX, float _fPosY,
                               float _fFontSize, UINT _FillColor, UINT _OutlineColor,
                               float _fThickness)
{
	// 캐시된(또는 새로 생성된) TextLayout 획득
	ComPtr<IDWriteTextLayout> pTextLayout;
	if (FAILED(GetOrCreateCachedTextLayout(
		m_FontPath,
		m_FontFamily,
		_pStr,
		_fFontSize,
		1000.f,    // 필요에 맞게 조정 가능
		200.f,     // 필요에 맞게 조정 가능
		pTextLayout.GetAddressOf())))
	{
		// 레이아웃 생성 실패 시 폴백: 기존 DrawString 방식으로 처리
		const float offsets_fb[8][2] =
		{
			{-1.f, -1.f}, { 0.f, -1.f}, { 1.f, -1.f},
			{-1.f,  0.f},               { 1.f,  0.f},
			{-1.f,  1.f}, { 0.f,  1.f}, { 1.f,  1.f}
		};

		for (int i = 0; i < 8; ++i)
		{
			m_FontWrapper->DrawString(
				CONTEXT, _pStr, _fFontSize,
				_fPosX + offsets_fb[i][0] * _fThickness,
				_fPosY + offsets_fb[i][1] * _fThickness,
				_OutlineColor,
				FW1_RESTORESTATE);
		}

		m_FontWrapper->DrawString(
			CONTEXT, _pStr, _fFontSize,
			_fPosX, _fPosY,
			_FillColor,
			FW1_RESTORESTATE);

		return;
	}

	// 오프셋 배열 (기존과 동일)
	const float offsets[8][2] =
	{
		{-1.f, -1.f}, { 0.f, -1.f}, { 1.f, -1.f},
		{-1.f,  0.f},               { 1.f,  0.f},
		{-1.f,  1.f}, { 0.f,  1.f}, { 1.f,  1.f}
	};

	// 아웃라인 먼저 렌더 (TextLayout 기준 위치에 오프셋 적용)
	for (int i = 0; i < 8; ++i)
	{
		m_FontWrapper->DrawTextLayout(
			CONTEXT,
			pTextLayout.Get(),
			_fPosX + offsets[i][0] * _fThickness,
			_fPosY + offsets[i][1] * _fThickness,
			_OutlineColor,
			FW1_RESTORESTATE);
	}

	// 본체 렌더 — 아웃라인 위에 덮어 씀
	m_FontWrapper->DrawTextLayout(
		CONTEXT,
		pTextLayout.Get(),
		_fPosX,
		_fPosY,
		_FillColor,
		FW1_RESTORESTATE);
}

