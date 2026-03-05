#pragma once
#include "Asset.h"
#include "ATexture.h"
#include "ASprite.h"

class ATileMap :
    public Asset
{
private:
    UINT                    m_Row;              // 타일맵의 행 개수
    UINT                    m_Col;              // 타일맵의 열 개수
    Vec2                    m_TileSize;         // 타일맵을 구성하는 타일 1개의 크기

    Ptr<ATexture>           m_Atlas;            // 타일맵을 구성하는 Sprite 들의 공통 아틀라스 텍스쳐
    vector<Ptr<ASprite>>    m_vecSpriteInfo;

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


    //=========
    // Get, Set
    //=========
    void SetRowCol(UINT _Row, UINT _Col);
    void SetSprite(UINT _Row, UINT _Col, Ptr<ASprite> _Sprite);
    GET_SET(Vec2, TileSize);
    GET_SET(Ptr<ATexture>, Atlas);
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }
    const vector<Ptr<ASprite>>& GetSprites() { return   m_vecSpriteInfo; }


    //============
    // 생성, 소멸자
    //============
    ATileMap();
    virtual ~ATileMap();
};
