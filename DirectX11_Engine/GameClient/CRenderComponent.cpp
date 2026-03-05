#include "pch.h"
#include "CRenderComponent.h"
#include "LevelMgr.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: Component(_Type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: Component(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	// Asset은 같은 주소를 가리켜도 상관없지만
	// 같은 동적 재질을 가리켜선 안된다.

	if (_Origin.m_Mtrl == _Origin.m_SharedMtrl)
		m_Mtrl = m_SharedMtrl;
	// 동적 재질이 존재하고, 현재 사용 중인 재질이 동적 재질이었다면
	else if (_Origin.m_DynamicMtrl != nullptr && _Origin.m_Mtrl == _Origin.m_DynamicMtrl)
		CreateDynamicMaterial();
	
	// 동적 재질을 생성 후, 사용하지 않은 상황에서, 생성해둔 동적 재질을 복사할 필요가 없다.
	// 필요하다면 새로 만들 것이기 때문
}

CRenderComponent::~CRenderComponent()
{
}

// Editer에서 Render된 오브젝트를 눈으로 봐야하기 때문에
// 게임 시작 시 실행되는 Begin이 아닌 Init에 Render를 배치
void CRenderComponent::Init()
{
	CreateMaterial();
}

void CRenderComponent::Begin()
{

}

void CRenderComponent::SetMaterial(Ptr<AMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<AMaterial> CRenderComponent::GetSharedMaterial()
{
	m_Mtrl = m_SharedMtrl;
	return m_SharedMtrl;
}

Ptr<AMaterial> CRenderComponent::CreateDynamicMaterial()
{
	// 동적 재질 생성은 반드시 Level이 Play 상태일 때만 사용이 가능하도록 예외처리
	assert(LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState());

	if (nullptr != m_DynamicMtrl)
	{
		m_Mtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}
	else
	{
		m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
		return m_DynamicMtrl;
	}
}
