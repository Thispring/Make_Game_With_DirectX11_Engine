#pragma once
#include "CRenderComponent.h"
#include "ASprite.h"

class CSpriteRender :
    public CRenderComponent
{

private:
    Ptr<ASprite>    m_Sprite;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;
    CLONE(CSpriteRender);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    void SetSprite(Ptr<ASprite> _Sprite) { m_Sprite = _Sprite; }
    Ptr<ASprite> GetSprite() { return m_Sprite; }


    //============
    // 생성, 소멸자
    //============
    CSpriteRender();
    // 기본 복사 생성자 사용
    virtual ~CSpriteRender();
};
