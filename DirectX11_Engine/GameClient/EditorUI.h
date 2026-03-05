#pragma once
#include "Entity.h"
#include "imgui/imgui.h"

class EditorUI :
    public Entity
{

private:
    string      m_UIName;       // 실제 UI의 이름
    string      m_UIKey;        // 자료구조에서 사용할 고유의 Key 이름

    bool        m_IsModal;      // Modal 방식 UI인지 여부
    bool        m_Separator;    // 구분선을 표시할지 여부, 기본은 true
    bool        m_Active;

    EditorUI*               m_Parent;   // 부모인 EditorUI를 가리키는 포인터 (자식이 부모를 가리킬 때는 일반 포인터 사용)
    vector<Ptr<EditorUI>>   m_ChildUI;  // 자신을 상속받은 자식들을 가리키는 포인터를 모아놓은 vector
    
    Vec2                    m_SizeAsChild; // 자식 UI 인 경우, 담당하는 영역 크기


    //=================
    // private 멤버 함수
    //=================
    void CheckFocus();      // 어떤 윈도우를 선택하고 있는지 확인하는 함수

protected:
    // UI의 고유의 Key 문자열을 설정할 수 있는 함수
    // protected로 상속받은 자식 클래스에서만 접근하도록 설정
    void SetUIKey(const string& _Key) { m_UIKey = _Key; }

public:
    // ========
    // 멤버 함수
    // ========
    void AddChildUI(Ptr<EditorUI> _Child)
    {
        _Child->m_Parent = this;        // EditorUI를 상속받는 자식의 부모를 this로 설정
        m_ChildUI.push_back(_Child);    // 자식UI 등록
    }
  

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick();
    virtual void Tick_UI() = 0;
    virtual void Activate() {}
    virtual void Deactivate() {}


    //=========
    // Get, Set
    //=========
    void SetUIName(const string& _Name) { m_UIName = _Name; }
    string GetUIName() { return m_UIName; }

    Ptr<EditorUI> GetParentUI() { return m_Parent; }

    void SetSeparator(bool _Set) { m_Separator = _Set; }
    void SetModal(bool _Modal) { m_IsModal = _Modal; }
    bool IsActive() { return m_Active; }
    void SetActive(bool _Active) 
    {
        if (m_Active == _Active)
            return;

        m_Active = _Active;

        if (m_Active)
            Activate();
        else
            Deactivate();
    }
    GET_SET(Vec2, SizeAsChild);


    //============
    // 생성, 소멸자
    //============
    EditorUI(const string& _Name);
    virtual ~EditorUI();
};

typedef void(EditorUI::* DELEGATE_0)(void);
typedef void(EditorUI::* DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);