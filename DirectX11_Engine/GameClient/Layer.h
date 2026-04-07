#pragma once
#include "Entity.h"
#include "GameObject.h"

// Layer의 역할이란?
class Layer :
    public Entity
{

private:
    vector<Ptr<GameObject>>         m_vecParents;       // Layer에 소속된 최상위 부모타입 오브젝트
    vector<Ptr<GameObject>>         m_vecAllObjects;    // Layer에 소속된 모든(부모, 자식타입) 오브젝트
    int                             m_LayerIdx;         // Layer 본인의 인덱스 (0~31)

public:
    //=========
    // 멤버 함수
    //=========
    void ReleaseObject(Ptr<GameObject> _Object, int _Idx) 
    {  
        vector<Ptr<GameObject>>::iterator Iter = m_vecParents.begin();
        for (int i = 0; i < _Idx; i++)
            ++Iter;
        m_vecParents.erase(Iter);
    }
    void RegisterObject(Ptr<GameObject> _Object) { m_vecAllObjects.push_back(_Object); }
    void DeregisterObject() { m_vecAllObjects.clear(); }
    void DeregisterAsParent(Ptr<GameObject> _Object);
    void ClearAllObjects() { m_vecParents.clear(); m_vecAllObjects.clear(); }

    void AddObject(Ptr<GameObject> _Object);

    void Begin();
    void Tick();
    void FinalTick();
    void Render();


    //=========
    // Get, Set
    //=========
    // const &로 전달하는 이유는 
    // 복사 비용을 아끼면서, 원본 손상을 막기 위함
    const vector<Ptr<GameObject>>& GetParentObjects() { return m_vecParents; }
    const vector<Ptr<GameObject>>& GetAllObjects() { return m_vecAllObjects; }


    //============
    // 생성, 소멸자
    //============
    Layer();
    Layer(const Layer& _Origin);
    virtual ~Layer();


    //=============
    // friend class
    //=============
    friend class ALevel;
};
