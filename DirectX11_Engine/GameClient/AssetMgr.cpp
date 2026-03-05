#include "pch.h"

#include "AMesh.h"
#include "AGraphicShader.h"
#include "ATexture.h"
#include "AMaterial.h"

#include "AssetMgr.h"
#include "PathMgr.h"

AssetMgr::AssetMgr()
	: m_Changed(false)
{
}

AssetMgr::~AssetMgr()
{
}

void AssetMgr::AddAsset(const wstring& _Key, Ptr<Asset> _Asset)
{
	// 동일한 Key 로 등록된 적이 없어야 한다.
	assert(m_mapAsset[(UINT)_Asset->GetType()].find(_Key) == m_mapAsset[(UINT)_Asset->GetType()].end());

	// Key값 설정
	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)_Asset->GetType()].insert(make_pair(_Key, _Asset));

	// 추가 성공했으면 true 변경
	m_Changed = true;
}

Ptr<Asset> AssetMgr::FindAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<Asset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);

	if (iter == m_mapAsset[(UINT)_Type].end())
		return nullptr;

	return iter->second;
}

void AssetMgr::GetAssetNames(ASSET_TYPE _type, vector<wstring>& _vec)
{
	for (const auto& pair : m_mapAsset[(UINT)_type])
	{
		_vec.push_back(pair.first);
	}
}