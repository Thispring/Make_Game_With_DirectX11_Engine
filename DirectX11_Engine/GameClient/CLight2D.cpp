#include "pch.h"
#include "CLight2D.h"
#include "RenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: Component(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	// Transform 컴포넌트를 가진 오브젝트들이 움직일 수 있으므로
	// 매 프레임 마다 Pos 정보를 갱신
	m_Info.WorldPos = Transform()->GetWorldPos();
	// 빛의 방향을 우벡터로 설정
	m_Info.LightDir = Transform()->GetDir(DIR::RIGHT);

	RenderMgr::GetInst()->RegisterLight2D(this);
}
