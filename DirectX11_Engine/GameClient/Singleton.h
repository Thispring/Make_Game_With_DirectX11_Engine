#pragma once

/********************************************************************************
* 싱글턴 패턴은 프로그램에 특정 클래스의 인스턴스가 하나만 생성되도록 
* 보장하는 디자인 패턴입니다.
*
* 생성자는 자식에서만 호출 가능하게 protected로 설정합니다.
* 
* 이 프로젝트에서 싱글턴은 힙 메모리에 객체를 생성 후, atexit()에 Destroy 함수를 등록하여
* 프로그램이 종료되어도 동적 생성된 객체를 소멸할 수 있도록 설계했습니다.
********************************************************************************/
template <typename T>
class Singleton
{
private:
	static T* m_This;

public:
	static T* GetInst()
	{
		if (nullptr == m_This)
			m_This = NEW T;

		return m_This;
	}

	static void Destroy()
	{
		if (nullptr != m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

protected:
	Singleton() 
	{
		/****************************************************************************
		* atexit는 C++ 라이브러리 함수입니다.
		* 호출되면 매개변수에 있는 함수의 주소를 전달하고
		* 프로그램 종료 시 해당 주소의 함수를 실행합니다.
		*
		* 반환 타입이 void이고 인자도 아무것도 안받는 그런 함수의 주소만 받을 수 있습니다.
		* + 함수 호출규약이 __cdecl 인 경우만 가능합니다.
		* 생성자에 atexit함수를 선언하고 Destory 함수의 주소를 등록해서, 
		* 프로그램 종료 시 Destory 함수를 호출합니다.
		****************************************************************************/
		atexit(Destroy);	
	}
};

template<typename T>
T* Singleton<T>::m_This = nullptr;