#pragma once
#include "EditorUI.h"
#include "TreeUI.h"     // 오브젝트가 자식을 들고 있을 수 있기에 Tree구조 UI를 사용하는 클래스를 만들어서 사용합니다.
#include "GameObject.h"

// Level에 있는 오브젝트의 정보를 표시하는 역할
class Outliner :
    public EditorUI
{
    
private:
    Ptr<TreeUI>             m_Tree;


    //=================
    // private 멤버 함수
    //=================
    void AddGameObject(Ptr<TreeNode> _ParentNode, Ptr<GameObject> _Object);
    void SelectGameObject(DWORD_PTR _Object);
    void AddChild(DWORD_PTR _Src, DWORD_PTR _Dest);

public:
    //=========
    // 멤버 함수
    //=========
    // Tree 갱신, 현재 Level의 최신 상태를 Tree에 표시
    void Renew();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    Outliner();
    virtual ~Outliner();
};
