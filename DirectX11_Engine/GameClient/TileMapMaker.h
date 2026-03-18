#pragma once
#include "EditorUI.h"
#include "ASprite.h"

class TileMapMaker :
    public EditorUI
{

private:
    //=============
    // Make TileMap
    //=============
    vector<Ptr<ASprite>>     m_vecSprite;    // UI에 표시할 Sprite 목록

    // ImGui에 호환되도록 UINT인 Row, Col을 해당 클래스에서는 int로 사용합니다.
    // 최종 Asset 저장 시, Row와 Col이 음수거나 오버플로가 발생할 시, assert로 예외처리
    int                     m_Row;
    int                     m_Col;
    Vec2                    m_TileSize;
    wstring                 m_TileMapName;
    wstring                 m_AtlasName;


    bool                    m_IsShowPreTile;
    bool                    m_IsShowInit;

    //=================
    // private 멤버 함수
    //=================
    void ClearSetting();

public:
    //=========
    // 멤버 함수
    //=========


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    GET_SET(int, Row);
    GET_SET(int, Col);
    GET_SET(Vec2, TileSize);
    GET_SET(wstring, TileMapName);
    GET_SET(wstring, AtlasName);


    //============
    // 생성, 소멸자
    //============
    TileMapMaker();
    virtual ~TileMapMaker();
};
