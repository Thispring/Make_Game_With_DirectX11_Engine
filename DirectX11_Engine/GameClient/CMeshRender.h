#pragma once
#include "CRenderComponent.h"
#include "assets.h"

class CMeshRender :
    public CRenderComponent
{

private:

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;
    CLONE(CMeshRender);
    

    //============
    // 생성, 소멸자
    //============
    CMeshRender();
    // 기본 복사 생성자 사용
    virtual ~CMeshRender();
};
