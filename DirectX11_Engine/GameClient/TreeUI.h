#pragma once
#include "EditorUI.h"

#pragma region Struct_TreeNode
// 트리 자료구조 방식으로 표시할 UI들을 연결하여 가리킵니다.
struct TreeNode
    : public Entity // Ptr 사용을 위한 상속
{
private:
    void ClickCheck();
    void DragCheck();
    void DropCheck();

public:
    //=====
    // 변수
    //=====
    string                  Str;
    string                  Key;

    bool                    Framed;

    DWORD_PTR               Data;       // UI가 표시하고 있는 정보
    class TreeUI*           m_Owner;    // 노드를 소유하고 있는 TreeUI

    TreeNode*               Parent;
    vector<Ptr<TreeNode>>   vecChildNode;

    //=====
    // 함수
    //=====
    void AddChildNode(Ptr<TreeNode> _Node)
    {
        // _Node를 TreeNode 자식 벡터에 집어넣고
        vecChildNode.push_back(_Node);
        // 부모포인터에 자신을 가리키게 한다.
        _Node->Parent = this;
    }
    void Tick();

    //=========
    // Get, Set
    //=========
    void SetFramed(bool _Frame) { Framed = _Frame; }

    //============
    // 생성, 소멸자
    //============
    TreeNode();
    virtual ~TreeNode();
};
#pragma endregion

#pragma region Class_TreeUI
class TreeUI :
    public EditorUI
{

private:
    vector<Ptr<TreeNode>>   m_vecNode;
    Ptr<TreeNode>           m_Selected; // 트리 소속 노드들 중에서 선택된 노드
    // Drag, Drop된 Node를 저장할 Ptr
    Ptr<TreeNode>           m_DropNode;
    Ptr<TreeNode>           m_DragNode;

    EditorUI*               m_Inst;
    DELEGATE_1              m_MemFunc;

    string                  m_DropKey;  // Payload 확인용 Key

    // Drag & Drop 데이터
    EditorUI*               m_DDInst;
    DELEGATE_2              m_DDMemFunc;

public:
    //=========
    // 멤버 함수
    //=========
    void AddDyanmicSelect(EditorUI* _Inst, DELEGATE_1 _MemFunc) { m_Inst = _Inst; m_MemFunc = _MemFunc; }
    void AddDyanmicDragDrop(EditorUI* _Inst, DELEGATE_2 _MemFunc) { m_DDInst = _Inst; m_DDMemFunc = _MemFunc; }
    
    // _ParentNode로 입력이 들어오면 특정 노드에 자식으로 추가, 만약 Node가 nullptr이라면?
    Ptr<TreeNode> AddItem(Ptr<TreeNode> _ParentNode, string _Str, DWORD_PTR _Data = 0);
    void Clear() { m_vecNode.clear(); }
    
    void RegisterSelected(Ptr<TreeNode> _Node);
    void RegisterDragged(Ptr<TreeNode> _Node) { m_DragNode = _Node; }
    void RegisterDropped(Ptr<TreeNode> _Node) { m_DropNode = _Node; }


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //=========
    // Get, Set
    //=========
    Ptr<TreeNode> GetSelected() { return m_Selected; }
    GET_SET(const string&, DropKey);


    //============
    // 생성, 소멸자
    //============
    TreeUI();
    virtual ~TreeUI();
};
#pragma endregion