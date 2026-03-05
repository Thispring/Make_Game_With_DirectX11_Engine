#pragma once
#include "EditorUI.h"
#include "GameObject.h"
#include "EComponentUI.h"
#include "EAssetUI.h"

class Inspector :
    public EditorUI
{

private:
    Ptr<GameObject>     m_TargetObject;
    Ptr<Asset>          m_TargetAsset;
    
    // Inspctor의 자식에 해당하는 UI 클래스를 빠르게 접근하기 위한 Ptr 멤버
    Ptr<EComponentUI>    m_arrComUI[(UINT)COMPONENT_TYPE::END];
    Ptr<EAssetUI>        m_arrAssetUI[(UINT)ASSET_TYPE::END];

public:
    //=========
    // 멤버 함수
    //=========
    void CreateChildUI();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    Ptr<GameObject> GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(Ptr<GameObject> _Object);

    Ptr<Asset> GetTargetAsset() { return m_TargetAsset; }
    void SetTargetAsset(Ptr<Asset> _Asset);


    //============
    // 생성, 소멸자
    //============
    Inspector();
    virtual ~Inspector();
};
