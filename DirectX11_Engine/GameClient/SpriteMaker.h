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
    wstring     m_SpriteName;
    int         m_OriIdx;
    int         m_Row;
    int         m_Col;
    int         m_EndOriIdx;


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
    GET_SET(wstring, TextureName);
    GET_SET(Vec2, SliceUV);
    GET_SET(int, StartLoop);
    GET_SET(int, EndLoop);
    GET_SET(wstring, SpriteName);
    GET_SET(int, OriIdx);
    GET_SET(int, Row);
    GET_SET(int, Col);
    GET_SET(int, EndOriIdx);


    //============
    // 생성, 소멸자
    //============
    SpriteMaker();
    virtual ~SpriteMaker();
};
