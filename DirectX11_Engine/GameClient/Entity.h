#pragma once

// Entity는 상속 개념을 활용하여,
// Asset에게 상속, 최상위 관리자 역할을 합니다.
class Entity
{
	static UINT g_NextID;
private:
	const UINT	m_InstID;		// 객체마다 가지는 고유 ID 값, 중복 방지를 위함
	wstring		m_Name;			// 객체마다 이름을 지정할 수 있다. 중복 허용(m_InstID로 구별)
	int			m_RefCount;		// 참조 카운팅

	void AddRef() { ++m_RefCount; }
	void Release()
	{
		--m_RefCount;
		if (m_RefCount <= 0)
			delete this;
	}

public: 
	//=========
	// Get, Set
	//=========
	UINT GetID() { return m_InstID; }
	void SetName(const wstring _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }


	//============
	// 생성, 소멸자
	//============
	Entity();
	Entity(const Entity& _Other);
	//Entity(const Entity& _Other) = delete;	// 컴파일러에게 복사생성자를 만들지 않게 알림
	virtual ~Entity();


	//=============
	// friend class
	//=============
	template<typename T>
	friend class Ptr;
};
