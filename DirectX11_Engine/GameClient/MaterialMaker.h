#pragma once
#include "EditorUI.h"

class MaterialMaker :
    public EditorUI
{

private:
    wstring         m_MtrlName;
    wstring         m_ShaderName;
    wstring         m_TextureName;
    RENDER_DOMAIN   m_RenderDomain;
    
    //=================
    // private 멤버 함수
    //=================
    void ClearSetting();

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    GET_SET(wstring, MtrlName);
    GET_SET(wstring, ShaderName);
    GET_SET(wstring, TextureName);
    GET_SET(RENDER_DOMAIN, RenderDomain);

    
    //============
    // 생성, 소멸자
    //============
    MaterialMaker();
    virtual ~MaterialMaker();
};
