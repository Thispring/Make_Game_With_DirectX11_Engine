#pragma once
#include "EditorUI.h"

// 원하는 아틀라스 텍스쳐에서 해상도 및 개수를 정하여
// Sprite 파일로 만들어주는 UI 클래스
class SpriteMaker :
    public EditorUI
{

private:
    //============
    // Make Sprite
    //============
    wstring     m_TextureName;
    Vec2        m_SliceUV;
    int         m_StartLoop;
    int         m_EndLoop;
    wstring     m_Path;
    int         m_OriIdx;

    //==============
    // Make Flipbook
    //==============
    wstring     m_SpriteName;
    wstring     m_FlipbookName;
    int         m_FlipLoop;

    //=============
    // Make TileMap
    //=============


public:
    //=========
    // 멤버 함수
    //=========
    void SpriteSettingClear();
    void FlipbookSettingClear();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;

    
    //=========
    // Get, Set
    //=========
    GET_SET(wstring, TextureName);
    GET_SET(Vec2, SliceUV);
    GET_SET(int, StartLoop);
    GET_SET(int, EndLoop);
    GET_SET(wstring, Path);
    GET_SET(int, OriIdx);

    GET_SET(wstring, SpriteName);
    GET_SET(wstring, FlipbookName);
    GET_SET(int, FlipLoop);


    //============
    // 생성, 소멸자
    //============
    SpriteMaker();
    virtual ~SpriteMaker();
};
