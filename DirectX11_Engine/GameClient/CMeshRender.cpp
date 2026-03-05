#include "pch.h"
#include "CMeshRender.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER) 
{
}

CMeshRender::~CMeshRender() 
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render() 
{
	// Mesh 혹은 Shader가 미설정인 상태
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;
	
	GetMaterial()->Binding();

	GetMesh()->Render();

	GetMaterial()->Clear();
}

void CMeshRender::CreateMaterial()
{
}
