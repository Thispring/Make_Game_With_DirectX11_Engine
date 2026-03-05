#pragma once
#include "Component.h"
#include "AMesh.h"
#include "AMaterial.h"

class CRenderComponent :
    public Component
{

private:
    Ptr<AMesh>          m_Mesh;
    Ptr<AMaterial>      m_Mtrl;         // 현재 사용중인 재질
    Ptr<AMaterial>      m_SharedMtrl;   // 공유 재질(AssetMgr이 관리)
    Ptr<AMaterial>      m_DynamicMtrl;  // 동적 재질(쓰고 버리는 용도)

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Init() override;
    // Begin함수에서 자식들의 순수 가상 함수를 호출
    virtual void Begin() override;

    // 랜더 기능
    virtual void Render() = 0;

    // 자신이 사용할 재질 로딩 및 생성
    virtual void CreateMaterial() = 0;

    // 추상 클래스는 Clone 구현 X

    //=========
    // Get, Set
    //=========
    GET_SET(Ptr<AMesh>, Mesh);
    Ptr<AMaterial> GetMaterial() { return m_Mtrl; }
    void SetMaterial(Ptr<AMaterial> _Mtrl);
    Ptr<AMaterial> GetSharedMaterial();
    Ptr<AMaterial> CreateDynamicMaterial();


    //============
    // 생성, 소멸자
    //============
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    virtual ~CRenderComponent();
};
