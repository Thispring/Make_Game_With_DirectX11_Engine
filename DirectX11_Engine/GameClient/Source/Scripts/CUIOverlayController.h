#pragma once
#include "CScript.h"

// UI 카메라에 렌더 대상인 게임 오브젝트들의 위치를
// 조절하는 스크립트 입니다.

class CUIOverlayController :
    public CScript
{

private:
    bool                        m_bInit;

    vector<Ptr<GameObject>>     m_vecUIObject;
    vector<Vec3>                m_vecUIOffset;  // 카메라 기준 초기 화면 오프셋 (X, Y)

public:
    void ChangeKeyUI();
    void UpdateMoveKeyUI();
    void UpdateHPBarUI();   // 자식 8, 9번 MeshRender 재질의 FILL 값을 HP 비율로 갱신

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    // Script를 상속 받았다면 Tick을 필수로 선언해야 함
    virtual void Tick() override;
    CLONE(CUIOverlayController);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    void SetInit() { m_bInit = true; }


    //============
    // 생성, 소멸자
    //============
    CUIOverlayController();
    virtual ~CUIOverlayController();
};

