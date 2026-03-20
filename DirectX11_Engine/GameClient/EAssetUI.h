#pragma once
#include "EditorUI.h"
#include "Asset.h"

class EAssetUI :
    public EditorUI
{

private:
    const ASSET_TYPE  m_AssetType;
    Ptr<Asset>        m_TargetAsset;

    std::chrono::steady_clock::time_point   m_MessageStartTime;
    wstring                                 m_SaveMessage;
    bool                                    m_IsShowSaveMessage;

protected:
    void OutputTitle();
    void ShowSaveMessage(const wstring& _Msg);
    void UpdateMessage();

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
