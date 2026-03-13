#pragma once
#include "EditorUI.h"

class GameViewportUI :
    public EditorUI
{
private:
    ComPtr<ID3D11ShaderResourceView>    m_GameSRV;  // 게임 렌더 타겟의 SRV
    Vec2                                m_LastViewportSize;  // 이전 프레임의 뷰포트 크기

public:
    //=========
    // 멤버 함수
    //=========
    void SetGameTexture(ComPtr<ID3D11ShaderResourceView> srv) { m_GameSRV = srv; }

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;

    //============
    // 생성, 소멸자
    //============
    GameViewportUI();
    virtual ~GameViewportUI();
};