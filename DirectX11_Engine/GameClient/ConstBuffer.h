#pragma once
#include "Entity.h"

// 상수 버퍼란?
class ConstBuffer :
    public Entity
{

private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    CB_TYPE                 m_Type;

public:
    //=========
    // 멤버 함수
    //=========
    int Create(CB_TYPE _Type, UINT _Size);
    void SetData(void* _SysMem, UINT _Size = 0);
    void Binding();


    //============
    // 생성, 소멸자
    //============
    ConstBuffer();
    virtual ~ConstBuffer();
};
