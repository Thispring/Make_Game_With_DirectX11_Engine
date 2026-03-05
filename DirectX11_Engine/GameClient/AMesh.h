#pragma once
#include "Asset.h"

// Mesh는 정점, 인덱스 정보로 구성된 모델 데이터
class AMesh :
    public Asset
{

private:
    ComPtr<ID3D11Buffer>    m_VB;           // 정점(Vertex) 버퍼
    D3D11_BUFFER_DESC       m_VBDesc;       // 정점 버퍼 생성 옵션
    UINT                    m_VtxCount;     // 정점 개수
    Vtx*                    m_VtxSysMem;    // 정점 데이터를 시스템 메모리에 저장

    ComPtr<ID3D11Buffer>    m_IB;           // 인덱스 버퍼
    D3D11_BUFFER_DESC       m_IBDesc;       // 인덱스 버퍼 생성 옵션
    UINT                    m_IdxCount;     // 인덱스 개수
    UINT*                   m_IdxSysMem;    // 인덱스 데이터를 시스템 메모리에 저장

public:
    //=========
    // 멤버 함수
    //=========
    int Create(Vtx* _VtxSysTem, UINT _VtxCount, UINT* _IdxSysTem, UINT _IdxCount);
    void Binding();
    void Render();


    //=========
    // Get, Set
    //=========
    const Vtx* GetVtxSysMem() { return m_VtxSysMem; }
    const UINT* GetIdxSysMem() { return m_IdxSysMem; }
    UINT GetVtxCount() { return m_VtxCount; }
    UINT GetIdxCount() { return m_IdxCount; }


    //============
    // 생성, 소멸자
    //============
    AMesh();
    virtual ~AMesh();
};
