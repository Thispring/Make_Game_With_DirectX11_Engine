#include "pch.h"
#include "GameViewportUI.h"
#include "RenderMgr.h"

GameViewportUI::GameViewportUI()
    : EditorUI("GameViewport")
    , m_LastViewportSize(0.f, 0.f)
{
}

GameViewportUI::~GameViewportUI()
{
}

void GameViewportUI::Tick_UI()
{
    ImGui::Begin("Game Viewport");

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    // 16:9 비율 유지 계산
    float targetAspect = 16.0f / 9.0f;
    float windowAspect = windowSize.x / windowSize.y;

    ImVec2 imageSize;
    ImVec2 imagePos = ImGui::GetCursorScreenPos();

    if (windowAspect > targetAspect)
    {
        // 윈도우가 더 넓음 -> Pillarbox (좌우에 검은 바)
        imageSize.y = windowSize.y;
        imageSize.x = imageSize.y * targetAspect;
        imagePos.x += (windowSize.x - imageSize.x) * 0.5f;
    }
    else
    {
        // 윈도우가 더 높음 -> Letterbox (상하에 검은 바)
        imageSize.x = windowSize.x;
        imageSize.y = imageSize.x / targetAspect;
        imagePos.y += (windowSize.y - imageSize.y) * 0.5f;
    }

    // 렌더 타겟은 항상 16:9로 고정
    Vec2 fixedResolution = Vec2(1920, 1080); // 또는 원하는 16:9 해상도

    if (m_LastViewportSize != fixedResolution)
    {
        m_LastViewportSize = fixedResolution;
        RenderMgr::GetInst()->ResizeGameRenderTarget(m_LastViewportSize);
        m_GameSRV = RenderMgr::GetInst()->GetGameSRV();
    }

    // 중앙 정렬된 위치에 이미지 그리기
    ImGui::SetCursorScreenPos(imagePos);
    if (m_GameSRV.Get())
    {
        ImGui::Image((ImTextureID)m_GameSRV.Get(), imageSize);
    }

    ImGui::Text("Window: %.0fx%.0f, Image: %.0fx%.0f",
        windowSize.x, windowSize.y, imageSize.x, imageSize.y);

    ImGui::End();
    //ImGui::Begin("Game Viewport");

    //// 현재 사용 가능한 영역 크기 가져오기
    //ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    //// 뷰포트 크기가 변경되었는지 확인
    //if (m_LastViewportSize.x != viewportSize.x || 
    //    m_LastViewportSize.y != viewportSize.y)
    //{
    //    // 크기가 유효한 경우에만 업데이트
    //    if (viewportSize.x > 0 && viewportSize.y > 0)
    //    {
    //        m_LastViewportSize = Vec2(viewportSize.x, viewportSize.y);
    //        
    //        // RenderMgr에 새 해상도 전달하여 렌더 타겟 재생성
    //        RenderMgr::GetInst()->ResizeGameRenderTarget(m_LastViewportSize);
    //        
    //        // 텍스처 갱신
    //        m_GameSRV = RenderMgr::GetInst()->GetGameSRV();
    //    }
    //}

    //// 게임 화면 표시
    //if (m_GameSRV.Get() && viewportSize.x > 0 && viewportSize.y > 0)
    //{
    //    ImGui::Image((ImTextureID)m_GameSRV.Get(), viewportSize);
    //    
    //    // 선택사항: 현재 뷰포트 크기 표시
    //    ImGui::Text("Viewport Size: %.0f x %.0f", viewportSize.x, viewportSize.y);
    //}
    //else
    //{
    //    ImGui::Text("Game Rendering Texture Not Available");
    //}

    //ImGui::End();
}