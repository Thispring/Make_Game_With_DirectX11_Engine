#pragma once

// Ptr은 프로젝트에 사용하기 위해, 자체적으로 만드는 스마트 포인터 역할을 합니다.
// 자료형을 자유롭게 가리키기 위해 템플릿으로 구현합니다.
template<typename T>
class Ptr
{
private:
	T* m_Ptr;

public:
	//=========
	// 멤버 함수
	//=========
	T* Get() const { return m_Ptr; }
	T** GetAdressOf() { return &m_Ptr; }


	//==============
	// 연산자 오버로딩
	//==============
	/****************************************************
	* 선언하지 않으면, 컴파일러가 복사생성자를 재활용하기 때문에
	* 명시적으로 대입연산자를 재정의합니다.
	****************************************************/
	Ptr& operator=(T * _Ptr)
	{
		// =호출되는 시점에 이미 가리키고 있다면
		if (m_Ptr != nullptr)
			m_Ptr->Release(); 

		m_Ptr = _Ptr;

		if (m_Ptr != nullptr)
			m_Ptr->AddRef();

		return *this;
	}

	// 같은 스마트 포인터끼리 대입연산자 재정의
	Ptr& operator=(const Ptr<T>& _Ptr)
	{
		if (m_Ptr != nullptr)
			m_Ptr->Release();

		m_Ptr = _Ptr.m_Ptr;

		if (m_Ptr != nullptr)
			m_Ptr->AddRef();

		return *this;
	}

	// -> 연산자 재정의
	T* operator->() const { return m_Ptr; }

	// 비교 연산 재정의
	// T 포인터 타입과의 비교와 스마트 포인터 끼리의 비교 재정의
	bool operator==(T* _Ptr) const { return m_Ptr == _Ptr; }
	bool operator==(const Ptr<T>& _Ptr) const { return m_Ptr == _Ptr.m_Ptr; }

	bool operator!=(T* _Ptr) const { return m_Ptr != _Ptr; }
	bool operator!=(const Ptr<T>& _Ptr) const { return m_Ptr != _Ptr.m_Ptr; }


	//============
	// 생성, 소멸자
	//============
	Ptr()
		: m_Ptr(nullptr)
	{}

	// T 포인터 타입 복사생성자
	Ptr(T* _Ptr)
		: m_Ptr(_Ptr)
	{
		if (m_Ptr != nullptr)
			m_Ptr->AddRef();
	}
	
	// 스마트포인터 복사생성자
	Ptr(const Ptr<T>& _Ptr)
		: m_Ptr(_Ptr.m_Ptr)
	{
		if (m_Ptr != nullptr)
			m_Ptr->AddRef();
	}

	~Ptr()
	{
		if (m_Ptr != nullptr)
			m_Ptr->Release();
	}
};

/***********************************************
* 일반 포인터 타입 변수와의 대입연산자 재정의
* 어떤 포인터 변수를 받을지 모르기 때문에 void* 사용
***********************************************/
template<typename T>
bool operator== (void* _Pointer, const Ptr<T>& _Ptr)
{
	return _Pointer == _Ptr.Get();
}

template<typename T>
bool operator!= (void* _Pointer, const Ptr<T>& _Ptr)
{
	return _Pointer != _Ptr.Get();
}
