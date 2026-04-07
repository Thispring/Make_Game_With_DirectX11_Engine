#include "pch.h"
#include "UIMgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imguiFunc.h"

#include "KeyMgr.h"
#include "LevelMgr.h"

#include "Engine.h"
#include "Device.h"

UIMgr::UIMgr()
    : m_ShowOptions(false)
    , m_isFullScreen(false)
    , m_ShowExitConfirm(false)  // ← 초기화 추가

    , BGMvolume(0.f)
    , SFXvolume(0.f)

{

}

UIMgr::~UIMgr()
{
}

void UIMgr::Init()
{

}

void UIMgr::Progress()
{
    // Exit? 확인창이 열려 있을 때는 ESC 토글 무시
    if (KEY_TAP(KEY::ESC) && !m_ShowExitConfirm)
    {
        m_ShowOptions = !m_ShowOptions;

        if (m_ShowOptions)
            ImGui::OpenPopup("OptionsWindow");
    }

    // 둘다 열리지 않게 방지
    if (KEY_TAP(KEY::CTRL))
    {
        m_ShowCredit = !m_ShowCredit;

        if (m_ShowCredit)
            ImGui::OpenPopup("CreditWindow");
    }

    RenderOptionsWindow();
    RenderCreditWindow();
}

void UIMgr::RenderOptionsWindow()
{
    if (!m_ShowOptions)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowSize(ImVec2(800.f, 450.f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize;

    if (ImGui::BeginPopupModal("OptionsWindow", nullptr, flags))
    {
        #pragma region Option Title
        Vec4 vColor = ColorConvertIntToVec4(3.f, 157.f, 252.f);
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, vColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
        ImGui::Button("Options", Vec2(150.f, 25.f));
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        #pragma endregion
        SPACING_UI(10);

        #pragma region FullScreen
        if (ImGui::Checkbox("FullScreen", &m_isFullScreen))
        {
            Engine::GetInst()->ToggleFullScreen();
        }
        #pragma endregion
        SPACING_UI(10);

        #pragma region OptionCloseBtn
        if (ImGui::Button("Close"))
        {
            m_ShowOptions = false;
            ImGui::CloseCurrentPopup();
        }
        #pragma endregion
        SPACING_UI(10);

        if (ImGuiFunc::ColoredButton("Exit",
            ColorConvertIntToVec4(38, 74, 27), ImVec2(120.f, 50.f)))
        {
            m_ShowExitConfirm = true;
            ImGui::OpenPopup("Exit?");
        }
    

        ImGui::EndPopup(); // ← 부모 EndPopup 나중
    }


    #pragma region ExitBtn
    // ── ImGui가 ESC로 닫았을 때 m_ShowExitConfirm이 true면 즉시 재오픈 ──
    if (m_ShowExitConfirm && !ImGui::IsPopupOpen("Exit?"))
    {
        ImGui::OpenPopup("Exit?");
    }

    // ── 중첩 팝업: 반드시 부모 EndPopup() 이전에 위치 ────────
    ImVec2 exitCenter(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(exitCenter, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Exit?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Do you want to Exit?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            m_ShowExitConfirm = false;  // ← 플래그 해제
            PostQuitMessage(0);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            m_ShowExitConfirm = false;  // ← 플래그 해제
            m_ShowOptions = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup(); // ← 자식 EndPopup 먼저
    }
    #pragma endregion
}

void UIMgr::RenderCreditWindow()
{
    // 모달 방식으로 구현, 위치 고정
    // 현재 Level을 일시정지로 전환
    if (!m_ShowCredit)
        return;

    // ── 1. 화면 중앙에 위치 고정 ──────────────────────────────
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

    // SetNextWindowPos(위치, 조건, 피벗)
    // ImGuiCond_Always : 매 프레임 강제 고정
    // pivot (0.5, 0.5) : 창의 중심을 기준으로 정렬
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowFocus(); // 강제 포커스 (최상위 입력 처리 보장)

    // ── 크기 지정 ──────────────────────────────────────────────
    // ImGuiCond_Always : 매 프레임 강제 고정
    // ImGuiCond_Once   : 처음 한 번만 적용 (이후 사용자가 조절 가능)
    ImGui::SetNextWindowSize(ImVec2(800.f, 450.f), ImGuiCond_Always);

    // ── 2. 모달 팝업 시작 ─────────────────────────────────────
    // ImGuiWindowFlags_NoMove     : 드래그 이동 불가
    // ImGuiWindowFlags_NoResize   : 크기 조절 불가 (선택)
    // ImGuiWindowFlags_NoTitleBar : 타이틀 바 숨김    (선택)
    // ── AlwaysAutoResize 제거, NoResize 추가 ───────────────────
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize;   // AlwaysAutoResize 삭제
    // AlwaysAutoResize를 유지하면 SetNextWindowSize가 무시됨

    if (ImGui::BeginPopupModal("CreditWindow", nullptr, flags))
    {
        // 크기 조절을 위해 OutputTitle 함수 사용 X
        Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, vColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
        ImGui::Button("Credit", Vec2(150.f, 50.f));
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        SPACING_UI(10);

        // NOTE(26-04-07):
        // Credit창에는 사용했던 Asset 목록을 .txt 파일로 저장하여
        // ImGui에서 이를 읽어와 .txt 문자열을 출력합니다.


        if (ImGui::Button("Close"))
        {
            m_ShowCredit = false;
            ImGui::CloseCurrentPopup(); // 모달 닫기
        }
        SPACING_UI(10);

        ImGui::EndPopup();
    }
}
