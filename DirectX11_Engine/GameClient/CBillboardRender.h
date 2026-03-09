#pragma once
#include "CRenderComponent.h"

class CBillboardRender :
    public CRenderComponent
{

private:
    Vec2            m_BillboardScale;
    Ptr<ATexture>   m_Tex;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;
    CLONE(CBillboardRender);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //============
    // 생성, 소멸자
    //============
    CBillboardRender();
    // 기본 복사 생성자 사용
    virtual ~CBillboardRender();
};
