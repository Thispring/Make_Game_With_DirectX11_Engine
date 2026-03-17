#pragma once
#include "EditorUI.h"
#include "GameObject.h"

// EComponentUI 들을 관리하는 중간 부모 역할 클래스
class EComponentUI :
    public EditorUI
{

private:
	Ptr<GameObject>			m_Target;
	const COMPONENT_TYPE	m_ComType;

protected:
	void OutputTitle(const string& _Title);
	void OutputTitle(const string& _Title, ImVec4 _Color);	// 원하는 컬러 매개변수를 주는 버전
	bool DeleteComponent(COMPONENT_TYPE _Type);				// Component를 제거하는 함수

public:
	//=========
	// Get, Set
	//=========
	void SetTarget(Ptr<GameObject> _Object);
	Ptr<GameObject> GetTarget() { return m_Target; }
	void ChangeComponent();


	//============
	// 생성, 소멸자
	//============
	EComponentUI(COMPONENT_TYPE _Type, const string& _Name);
	virtual ~EComponentUI();
};
