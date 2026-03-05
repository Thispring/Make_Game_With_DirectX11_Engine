#include "pch.h"
#include "Entity.h"

UINT Entity::g_NextID = 0;

// m_RefCount는 참조되지 않은 상태면 0 유지
// 복사 생성자도 동일
Entity::Entity()
	: m_InstID(g_NextID++)
	, m_RefCount(0)
{
	/**********************************************************************
	* Entity 객체에 const 멤버가 있기 때문에
	* 객체의 디폴트 = 대입 연산자에 문법 오류가 발생합니다.
	* 단, 복사 생성자는 실행이 가능합니다.
	*  => 객체 생성 시, 초기화하는 방식으로 동작하기 때문에
	*
	* 디폴트 복사 생성자를 Entity의 구현 의도에 맞게 수정해야 합니다.
	* Entity는 똑같은 m_InstID를 가지면 안됩니다.
	*  => 객체의 구현 의도를 생각하여, 객체의 디폴트 로직들을 알맞게 변경해야 합니다.
	**********************************************************************/
}

Entity::Entity(const Entity& _Other)	
	: m_InstID(g_NextID++)
	, m_Name(_Other.m_Name)
	, m_RefCount(0)
{
}

Entity::~Entity()
{

}
