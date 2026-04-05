#include "pch.h"
#include "UIMgr.h"

#include "imgui/imgui.h"

#include "Engine.h"

UIMgr::UIMgr()
{

}

UIMgr::~UIMgr()
{
    // 생성한 Win32 버튼 윈도우를 모두 파괴
    for (auto& pair : m_mapButtons)
    {
        if (pair.second)
            DestroyWindow(pair.second);
    }
    m_mapButtons.clear();
    m_mapImGuiButtons.clear();
}

void UIMgr::Init()
{
    // 엔진 시작 시 항상 표시할 ImGui 버튼 등록 예시
    // CreateImGuiButton(20, 20, 160, 30, L"전체화면 (F5)", -1001);
}

// ────────────────────────────────────────────────────────────
// ImGui 프레임 안에서 매 프레임 호출 → 등록된 ImGui 버튼 렌더링
// EditorMgr::Tick() 내 NewFrame() 이후, ImGui::Render() 이전에 호출
// ────────────────────────────────────────────────────────────
void UIMgr::Tick()
{
    for (auto& [id, btn] : m_mapImGuiButtons)
    {
        btn.clicked = false;

        // 타이틀바·테두리 없이 정확한 좌표에 버튼 하나만 띄우는 창
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
                               | ImGuiWindowFlags_NoResize
                               | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoScrollbar
                               | ImGuiWindowFlags_NoScrollWithMouse
                               | ImGuiWindowFlags_NoSavedSettings
                               //| ImGuiWindowFlags_NoBringToDisplayOnFocus
                               | ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowPos(ImVec2((float)btn.x, (float)btn.y), ImGuiCond_Always);

        // WindowPadding 을 (0,0) 으로 해서 버튼이 창 크기 그대로 채워지게 함
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize,  ImVec2(0.f, 0.f));

        string wndId = "##UIMgrBtn_" + to_string(id);
        ImGui::Begin(wndId.c_str(), nullptr, flags);

        ImGui::PopStyleVar(2);

        if (ImGui::Button(btn.label.c_str(), ImVec2((float)btn.width, (float)btn.height)))
            btn.clicked = true;

        ImGui::End();
    }
}

// ────────────────────────────────────────────────────────────
// Win32 버튼
// ────────────────────────────────────────────────────────────
HWND UIMgr::CreateWindowButton(int x, int y, int width, int height,
                               const wstring& _label, int _id)
{
    HWND hExist = FindButton(_id);
    if (hExist)
        return hExist;

    HWND hParent = Engine::GetInst()->GetMainWndHwnd();
    HINSTANCE hInst = Engine::GetInst()->GetInstance();

    HWND hBtn = CreateWindowW(
        L"BUTTON",
        _label.c_str(),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height,
        hParent,
        (HMENU)(INT_PTR)_id,
        hInst,
        nullptr
    );

    if (hBtn)
        m_mapButtons.emplace(_id, hBtn);

    return hBtn;
}

HWND UIMgr::FindButton(int _id)
{
    auto iter = m_mapButtons.find(_id);
    if (iter == m_mapButtons.end())
        return nullptr;

    return iter->second;
}

void UIMgr::RemoveButton(int _id)
{
    auto iter = m_mapButtons.find(_id);
    if (iter == m_mapButtons.end())
        return;

    DestroyWindow(iter->second);
    m_mapButtons.erase(iter);
}

// ────────────────────────────────────────────────────────────
// ImGui 버튼 등록 / 조회 / 제거
// ────────────────────────────────────────────────────────────
bool UIMgr::CreateImGuiButton(int x, int y, int width, int height,
                              const wstring& _label, int _id)
{
    if (m_mapImGuiButtons.find(_id) != m_mapImGuiButtons.end())
        return false;   // 이미 등록된 ID

    // wstring → UTF-8 변환 (ImGui는 UTF-8 문자열 사용)
    int size = WideCharToMultiByte(CP_UTF8, 0, _label.c_str(), -1,
                                   nullptr, 0, nullptr, nullptr);
    string utf8(size - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, _label.c_str(), -1,
                        utf8.data(), size, nullptr, nullptr);

    ImGuiButtonData data;
    data.x = x;  data.y = y;
    data.width = width;  data.height = height;
    data.label = utf8;

    m_mapImGuiButtons.emplace(_id, data);
    return true;
}

bool UIMgr::IsImGuiButtonClicked(int _id) const
{
    auto iter = m_mapImGuiButtons.find(_id);
    if (iter == m_mapImGuiButtons.end())
        return false;

    return iter->second.clicked;
}

void UIMgr::RemoveImGuiButton(int _id)
{
    m_mapImGuiButtons.erase(_id);
}
