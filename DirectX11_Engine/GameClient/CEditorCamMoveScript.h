#pragma once
#include "CScript.h"
#include "GameObject.h"

class CEditorCamMoveScript :
    public CScript
{

private:
    Vec3    m_OriginPos;        // 초기 위치
    Vec3    m_OriginRot;        // 초기 회전
    Vec3    m_CurPos;           // 현재 위치
    Vec3    m_CurRot;           // 현재 회전

    bool    m_isMoving;         // 이동중인지 여부

    Ptr<GameObject> m_pPlayer;  // 추적 대상 플레이어
    bool    m_bFollowPlayer;    // F6 플레이어 추적 토글

public:
    //=========
    // 멤버 함수
    //=========
    void MoveOrigin();
    void MovingOrigin();
    void OrthoCamMove();
    void PrespecCamMove();
    void MouseCamMove();

    // Vec3의 각 요소의 절댓값을 계산하는 함수
    Vec3 Vec3Abs(const Vec3& v);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    virtual void Tick() override;

    CLONE(CEditorCamMoveScript);
    /***************************************************************************
    * EditorCam는 EditorMgr에서 초기화 단계에서 EditorCam 게임오브젝트를 생성하므로
    * 움직임을 제어하는 해당 Script Component 정보를 파일로 저장하지 않습니다.
    ***************************************************************************/
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CEditorCamMoveScript();
    virtual ~CEditorCamMoveScript();
};