#pragma once
#include "Asset.h"
#include "Layer.h"

class ALevel :
    public Asset
{
private:
    // 하나의 레벨안에 32개의 레이어가 존재
    Layer   m_arrLayer[MAX_LAYER];

    /*****************************************************
    * 레이어 끼리 충돌검사를 진행할지 마킹한 데이터
    * 행렬 계산을 통해 충돌검사를 진행할 레이어를 계산
    * 둘 중 작은 숫자를 행으로 사용 (행렬의 1/2만 사용하기 위함)
    * EX) Unity 엔진에서 제공하는 충돌 검사 UI
    *****************************************************/
    UINT    m_Matrix[MAX_LAYER];

    // Level 안에 오브젝트들의 상태 변경(추가, 삭제, 계층관계) 확인
    bool    m_Changed;          

public:
    //=========
    // 멤버 함수
    //=========
    void AddObject(int _LayerIdx, Ptr<GameObject> _Object);
    void DeleteObject(Ptr<GameObject> _Object);
    void Deregister();
    void CheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2);


    // 처음 시작했을 때 한 번 실행
    void Begin();
    // 매 프레임 실행
    void Tick();
    // 한 프레임에서 Tick이 끝날 때 실행
    void FinalTick();

    Ptr<GameObject> FindObjectByName(const wstring& _Name);

    // Level을 복사하는 함수, 호출자 자신의 주소를 전달하여 동적할당
    //ALevel* Clone() { return NEW ALevel(*this); }
    CLONE(ALevel);


    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


    //=========
    // Get, Set
    //=========
    Layer* GetLayer(int _Idx) 
    { 
        assert(0 <= _Idx && _Idx < MAX_LAYER);
        return &m_arrLayer[_Idx]; 
    }
    // Layer 배열 전체를 반환
    Layer* GetLayer() { return m_arrLayer; }
    UINT* GetCollisionMatrix() { return m_Matrix; };

    bool IsChanged()
    { 
        // 원본 bool 값을 바꾸어 변환하지 않는다
        bool Changed = m_Changed; 
        m_Changed = false; 
        return Changed; 
    }
    void SetChanged() { m_Changed = true; }


    //============
    // 생성, 소멸자
    //============
    ALevel();
    virtual ~ALevel();
};
