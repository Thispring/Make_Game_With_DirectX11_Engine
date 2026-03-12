#pragma once
#include "Entity.h"
#include "components.h"

// 자주사용하는 함수 매크로 정의
#define GET_COMPONENT(COM_NAME, COM_TYPE)		Ptr<C##COM_NAME> COM_NAME() { return (C##COM_NAME*)m_Com[(UINT)COMPONENT_TYPE::COM_TYPE].Get();}

class GameObject
	: public Entity
{

private:
	/************************************************************************************
	* GameObject가 제거될 때, 본인이 가지고 있던 Component 요소를 해제하기 위한 스마트포인터 선언
	* enum 값 번호를 확인하여 어떤 Component를 가지고 있는지 확인이 가능합니다.
	************************************************************************************/
	Ptr<Component>				m_Com[(UINT)COMPONENT_TYPE::END];

	// GameObject는 Render를 하나만 가질 수 있습니다. 이를 관리하기 위한 포인터
	Ptr<CRenderComponent>		m_RenderCom;

	// Script를 관리하는 vector
	vector<Ptr<CScript>>		m_vecScripts;

	// 자식 오브젝트를 관리하는 부모 포인터 (스마트 포인터와의 관계를 이용)
	GameObject*					m_Parent;
	
	// 자신의 자식오브젝트를 가리킬 포인터
	vector<Ptr<GameObject>>		m_vecChild;

	// GameObject 본인이 속한 LayerIndex, -1인 경우 어떤 레이어에도 속하지 않는다(레벨안에 있지 않은 오브젝트)
	int							m_LayerIdx;	

	// 오브젝트가 다음 프레임에 삭제될 지 여부
	bool						m_Dead;


	//=================
	// private 멤버 함수
	//=================
	void RegisterLayer();

public:
	//=========
	// 멤버 함수
	//=========

	// 레벨이 처음 시작될때 호출되는 함수
	void Begin();

	// 매 프레임 마다 할 일
	void Tick();

	// 매 프레임마다 Tick 이후에 뒷 수습작업을 수행
	void FinalTick();
	void FinalTick_Editor();

	// 자기 자신을 그려야 하는 일
	void Render();
	
	// 자신이 제거될 오브젝트인지 여부를 반환
	bool IsDead() { return m_Dead; }

	// 자신을 TaskMgr에게 제거해 달라고 요청하는 함수
	void Destroy();

	void AddComponent(Ptr<Component> _Com);

	// 부모, 자식 관계 등록 및 해제 함수
	void AddChild(Ptr<GameObject> _Child);
	void DisconnectWithParent();
	void RegisterAsParent();
	void DeregisterAsParent();

	// 자신이 가리키고 있던 Component를 해제하는 함수
	void ReleaseComponent(COMPONENT_TYPE _Type)
	{
		// Transform의 경우 기본으로 가지고 있어야 되기때문에 해제 X
		if (_Type == COMPONENT_TYPE::TRANSFORM)
			return;

		m_Com[(UINT)_Type] = nullptr;

		// _Type이 Render Component라면
		// m_RenderCom도 nullptr을 가리키게 설정
		if (_Type == COMPONENT_TYPE::BILLBOARD_RENDER || _Type == COMPONENT_TYPE::FLIPBOOK_RENDER || _Type == COMPONENT_TYPE::MESHRENDER ||
			_Type == COMPONENT_TYPE::PARTICLE_RENDER || _Type == COMPONENT_TYPE::SPRITE_RENDER || _Type == COMPONENT_TYPE::TILE_RENDER)
			m_RenderCom = nullptr;

	}

	// File I/O
	void SaveToLevelFile(FILE* _File);
	void LoadFromLevelFile(FILE* _File);


	//=========
	// Get, Set
	//=========
	Ptr<Component> GetComponent(COMPONENT_TYPE _Type) { return m_Com[(UINT)_Type]; }
	Ptr<CRenderComponent> GetRenderCom() { return m_RenderCom; }
	Ptr<GameObject> GetParent() { return m_Parent; }
	Ptr<GameObject> GetChild(int _Idx) { return m_vecChild[_Idx]; }
	const vector<Ptr<GameObject>>& GetChild() { return m_vecChild; }
	int GetLayerIdx() { return m_LayerIdx; }
	// LayerIdx를 사용자가 설정하게 변경
	void SetLayerIdx(int _Idx) { m_LayerIdx = _Idx; }


	vector<Ptr<CScript>> GetScripts() { return m_vecScripts; }
	// (26-02-23): GetScript를 template으로 변경하여
	// vector를 순회하여, T타입에 맞는 Script 주소를 반환
	template<typename T>
	Ptr<T> GetScript();

	// 매개변수로 받은 GameObject의 주소 반환
	Ptr<GameObject> GetGameObject(Ptr<GameObject> _Obj) { return _Obj; }
	
	// 객체 복제 함수
	CLONE(GameObject);


	/*****************************************************************
	* Component를 상속받은 각 기능의 주소를 받아와, 해당 기능 객체를 가리키는 
	* 포인터 타입으로 다운 캐스팅, 외부에서 편리하게 호출하기 위해 작성
	* 
	* 특정 컴포넌트를 다운캐스팅해서 바로 리턴
	*****************************************************************/
	// [ComponentAuto.exe]
	// "GET COMPONENT" 주석 문자 아래 자동 생성
	// GET COMPONENT
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileRender, TILE_RENDER);
    GET_COMPONENT(FlipbookRender, FLIPBOOK_RENDER);
    GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(BillboardRender, BILLBOARD_RENDER);
	GET_COMPONENT(Camera, CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);
	GET_COMPONENT(SpriteRender, SPRITE_RENDER);
	
	GET_COMPONENT(Script, SCRIPT);

	
	//============
	// 생성, 소멸자
	//============
	GameObject();
	GameObject(const GameObject& _Origin);
	virtual ~GameObject();


	//=============
	// friend class
	//=============
	friend class Layer;
	friend class TaskMgr;	// TaskMgr에 IsDead 상태를 확인 할 수 있게 friend 선언
};

/***************************************************
* IsDead인 오브젝트를 삭제하는 전역 함수
* func.h에서 GameObject 헤더 사용이 어려움으로 인해
* 전방선언은 GameObject.h에서, 기능은 fun.cpp에서 구현
***************************************************/
bool IsValid(Ptr<GameObject>& _Object);


template<typename T>
inline Ptr<T> GameObject::GetScript()
{
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		// 다운 캐스팅으로, CScript를 상속받은 파생 Script의 주소를 전달
		T* pScript = dynamic_cast<T*>(m_vecScripts[i].Get());
		if (pScript == nullptr)
			continue;

		return pScript;
	}


	return nullptr;
}
