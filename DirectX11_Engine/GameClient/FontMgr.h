#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib"))
#endif

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

//======================================================
// Dx11 라이브러리를 이용해서 폰트 레이아웃을 생성하기 위한 헤더
//======================================================
#include <dwrite_3.h>
#include <filesystem>
#include <algorithm>
#pragma comment(lib, "Dwrite.lib")

class FontMgr :
    public Singleton<FontMgr>
{
    SINGLE(FontMgr);
private:
    IFW1Factory*                m_FW1Factory;
    IFW1FontWrapper*            m_FontWrapper;
    HANDLE                      m_hFontResource = nullptr;
    std::vector<BYTE>           m_fontBuffer;

    wstring                     m_FontPath;
    wstring                     m_FontFamily;

    //=================
    // private 멤버 함수
    //=================
    void CreateFontWrapper();
    int FontCheck();

public:
    void OldInit();
    void Init(const wstring& _FontPath = L"", const wstring& _FontFamily = L"");

    void PrintEnding();
    void PrintGameOver();

    // _Color : FONT_RGBA 매크로
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);

    // 아웃라인 텍스트 그리기
    // _FillColor    : 텍스트 본체 색
    // _OutlineColor : 아웃라인 색
    // _fThickness   : 아웃라인 두께(픽셀)
    void DrawFontOutline(const wchar_t* _pStr, float _fPosX, float _fPosY,
                         float _fFontSize, UINT _FillColor, UINT _OutlineColor,
                         float _fThickness = 1.5f);

    // 텍스트 크기 측정 — 반환값: { 너비, 높이 } (픽셀 단위)
    Vec2 MeasureText(const wchar_t* _pStr, float _fFontSize);

    // FontLayout 생성
    HRESULT CreateTextLayoutFromFontFile(const std::wstring& InFontFilePath,
        const std::wstring& InFamilyName,
        const std::wstring& InText,
        float InFontSize,
        float InMaxWidth,
        float InMaxHeight,
        IDWriteTextLayout** OutTextLayout);
};

