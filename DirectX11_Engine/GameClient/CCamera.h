#pragma once
#include "Component.h"

class GameObject;

// 투영 방식 종류
enum class PROJ_TYPE
{
    ORTHOGRAPHIC,   // 직교투영
    /*********************************************************************************************************
    * EX)옛날 게임 중, 입체감을 주기위해 45도 각도로 내려다 보는 방식의 게임에서, 가까운 물체와 멀리있는 물체의 크기가 같다.
    * 이는 직교 투영 방식을 사용했기 때문
    *********************************************************************************************************/

    PERSPECTIVE,    // 원근투영
    // 직교투영과 반대로 현실적인 투영방식
};

class CCamera :
    public Component
{

private:
    UINT        m_LayerCheck;       // 어떤 레이어만 화면에 렌더링 할 것인지 비트를 사용해 체크
    PROJ_TYPE   m_ProjType;         // 투영 방식

    float       m_Far;              // 카메라 시야 최대거리
    float       m_Width;            // 투영 가로길이
    float       m_AspectRatio;      // 종횡비 (가로 / 세로), 세로대비 가로의 길이 비율
    float       m_FOV;              // 원근 투영 시야각
    float       m_OrthoScale;       // 직교 투명 배율

    bool        m_IsUICam;

    Matrix      m_matView;          // View 행렬
    Matrix      m_matProj;          // Proj 행렬

    // 렌더링 도메인 별로 vector에 저장하고 정렬
    vector<GameObject*>         m_vecOpaque;
    vector<GameObject*>         m_vecMasked;
    vector<GameObject*>         m_vecPostProcess;
    vector<GameObject*>         m_vecTransparent;

public:
    //=========
    // 멤버 함수
    //=========
    void LayerCheckAll();
    void LayerCheckClear();
    void LayerCheck(int _Idx);
    void Render();
    void SortObejct();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void FinalTick() override;

    CLONE(CCamera);
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    GET_SET(PROJ_TYPE, ProjType);
    GET_SET(float, Far);
    GET_SET(float, Width);
    GET_SET(float, AspectRatio);
    GET_SET(float, OrthoScale);
    GET_SET(bool, IsUICam);
    UINT GetLayerCheck() { return m_LayerCheck; }

    // 자신의 PROJ_TYPE 레퍼런스를 반환
    PROJ_TYPE& GetTypeRef() { return m_ProjType; }

    // FOV를 60분법이 아닌 라디안으로 구하기 위해
    // FOV의 Get, Set함수는 따로 구현
    float GetFOV() { return m_FOV * (180 / XM_PI); }  // 라디안을 60분법으로
    void SetFOV(float _Degree) { m_FOV = _Degree * (XM_PI / 180.f); }   // 60분법을 라디안으로


    // 마우스 좌표 획득은 윈도우를 보고 있는 카메라에서 담당합니다.
    // _ScreenPos : 클라이언트(윈도우) 기준 픽셀 좌표 (왼쪽 상단-origin)
    // _ZNormalized : 0..1 범위의 깊이 값 (0 = near plane, 1 = far plane). 2D에서는 0 사용 권장.
    Vec3 ScreenToWorldPos(const Vec2& _ScreenPos, float _ZNormalized = 0.f);


    //============
    // 생성, 소멸자
    //============
    CCamera();
    // 기본 복사 생성자 사용
    virtual ~CCamera();
};
