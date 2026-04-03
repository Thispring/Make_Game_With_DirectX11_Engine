#pragma once
#include "CScript.h"

class CUICamMoveScript :
    public CScript
{

private:
    Ptr<GameObject> m_Target;         // Pos를 추적할 타겟 오브젝트 (Player)

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() override;
    // Script를 상속 받았다면 Tick을 필수로 선언해야 함
    virtual void Tick() override;
    CLONE(CUICamMoveScript);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CUICamMoveScript();
    virtual ~CUICamMoveScript();
};

