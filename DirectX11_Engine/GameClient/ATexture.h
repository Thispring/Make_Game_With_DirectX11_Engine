#pragma once
#include "Asset.h"

class ATexture :
    public Asset
{

private:
    ScratchImage                        m_Image;    // Content 폴더에 있는 이미지 파일을 시스템 메모리로 불러들임
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // 시스템메모리로 로딩한 픽셀 데이터를 GPU 메모리로 전송
    D3D11_TEXTURE2D_DESC                m_Desc;

    // 텍스쳐의 용도에 맞는 View 들 선언
    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;

    int                                 m_RecentNum;
    
public:
    //=========
    // 멤버 함수
    //=========
    void Binding(UINT _RegisterNum);
    void Clear();   // 텍스쳐 바인딩 해제


    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;
    

    //=========
    // Get, Set
    //=========
    float GetWidth() { return m_Desc.Width; }
    float GetHeight() { return m_Desc.Height; }

    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }


    //============
    // 생성, 소멸자
    //============
    ATexture();
    virtual ~ATexture();
};
