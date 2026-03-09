#pragma once
#include "Entity.h"

// 자주 사용되는 함수 매크로 전방 선언
#define GET_OTHER_COMPONENT(COM_NAME) class C##COM_NAME* COM_NAME();

class GameObject;					// GameObject 헤더 참조를 대신할 전방선언(멤버 함수에서 GameObject 사용)

// Component란?
// Component를 상속받는 다른 class 이름 앞에 'C'를 붙여줍니다.
class Component
	: public Entity
{

private:
	const COMPONENT_TYPE m_Type;	// 어떤 종류의 Component인지를 구별하기 위한 멤버 변수
	GameObject* m_Owner;			// Component를 소유한 GameObject를 가리키는 포인터, Ptr이 아닌 일반 포인터로 선언(종속성 고려)

public:
	//=============
	// 상속 멤버 함수
	//=============
	virtual void Init() {};			// 자식 Component의 초기화 담당,
									// 추후 게임엔진을 Editer에서 게임을 실행하지 않아도 Render를 하기 위해 사용합니다. 
									// Component Render는 Begin이 아닌 Init에서 처리

	virtual void Begin() {}			// 오버라이딩 강제 X
	
	virtual void FinalTick() = 0;	// 오버라이딩 강제 O

	/************************************************************
	* Clone 함수를 virtual로 설정한 이유는?
	* 모든 Component 클래스를 상속 받고 있는 자식 Component들을
	* 수동으로 다운캐스팅하여 복사하지 않게 하기 위함
	* 
	* 가장 좋은 방법은 추상화를 하여, 오버라이딩을 강제하고
	* 상속받은 자식의 Clone 함수를 직접 구현하면서 점검하는것이 좋다.
	*  => (꼭 복사생성자가 필요한 멤버가 있는지 체크)
	************************************************************/
	virtual Component* Clone() = 0;

	/*************************************************************
	* 상속 받은 Component들을 파일 형태로 저장 및 불러오기 위한 함수
	* Component 별로 멤버 요소가 다르므로, 자식 클래스에서 내용을 구현
	*************************************************************/
	virtual void SaveToLevelFile(FILE* _File) = 0;
	virtual void LoadFromLevelFile(FILE* _File) = 0;


	//=========
	// Get, Set
	//=========
	COMPONENT_TYPE GetType() { return m_Type; }
	GameObject* GetOwner() { return m_Owner; }
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(TileRender);
    GET_OTHER_COMPONENT(FlipbookRender);
    GET_OTHER_COMPONENT(SpriteRender);
	GET_OTHER_COMPONENT(Transform);
	GET_OTHER_COMPONENT(MeshRender);
	GET_OTHER_COMPONENT(BillboardRender);
	GET_OTHER_COMPONENT(Camera);
	GET_OTHER_COMPONENT(Collider2D);
	
	GET_OTHER_COMPONENT(Script);

	
	//============
	// 생성, 소멸자
	//============
	Component(COMPONENT_TYPE _Type);	// COMPONENT_TYPE 타입만을 받는 생성자
	Component(const Component& _Origin);	
	virtual ~Component();


	//=============
	// friend class 
	//=============
	friend class GameObject;
};
