#include "pch.h"
#include "Asset.h"

Asset::Asset(ASSET_TYPE _Type)
	: m_Type(_Type)
{
}

Asset::Asset(const Asset& _Origin)
	: Entity(_Origin)	
	, m_Key(_Origin.m_Key)
	, m_RelativePath()
	, m_Type(_Origin.m_Type)
{
	/*****************************************************************************
	* 복사 생성자를 수동으로 작성할 때, 부모 클래스가 있다면, 부모의 생성자도 고려해야 합니다.
	*
	* 런타임 도중 메모리에서 복사되는 Asset의 경우
	* 자신이 어떤 경로를 가리키는지 Path를 비워줍니다.
	*****************************************************************************/
}

Asset::~Asset()
{
}
