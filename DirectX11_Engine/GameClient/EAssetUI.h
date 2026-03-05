#pragma once
#include "EditorUI.h"
#include "Asset.h"

class EAssetUI :
    public EditorUI
{

private:
    const ASSET_TYPE  m_AssetType;
    Ptr<Asset>        m_TargetAsset;

protected:
    void OutputTitle();

public:
    //=========
    // Get, Set
    //=========
    void SetTargetAsset(Ptr<Asset> _Asset) { m_TargetAsset = _Asset; }
    Ptr<Asset> GetTargetAsset() { return  m_TargetAsset; }


    //============
    // 생성, 소멸자
    //============
    EAssetUI(ASSET_TYPE _Type);
    virtual ~EAssetUI();
};
