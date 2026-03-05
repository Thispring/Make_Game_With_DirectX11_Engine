#pragma once
#include "CRenderComponent.h"
#include "ATileMap.h"
#include "StructuredBuffer.h"

struct SpriteInfo
{
    Vec2 LeftTop;
    Vec2 Slice;
};

class CTileRender :
    public CRenderComponent
{

private:
    Ptr<ATileMap>           m_TileMap;
    vector<SpriteInfo>      m_vecSpriteInfo;
    Ptr<StructuredBuffer>   m_Buffer;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;
    CLONE(CTileRender);


    //=========
    // Get, Set
    //=========
    void SetTileMap(Ptr<ATileMap> _TileMap);


    //===========
    // 생성, 소멸자
    //===========
    CTileRender();
    CTileRender(const CTileRender& _Origin);
    virtual ~CTileRender();
};
