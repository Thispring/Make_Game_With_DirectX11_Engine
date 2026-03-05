#include "pch.h"
#include "ASprite.h"
#include "AssetMgr.h"

ASprite::ASprite()
	: Asset(ASSET_TYPE::SPRITE)
{
}

ASprite::~ASprite()
{
}

int ASprite::Save(const wstring& _FilePath)
{
	/******************************************************
	* Sprite 객체가 가진 멤버 중, 자신이 어떤 Texture를
	* 가리키고 있는지, Texture를 UV 좌표 기준 얼마만큼 자를지에
	* 대한 멤버의 정보를 저장합니다.
	******************************************************/
	
	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	FILE_OPEN(_FilePath, L"wb");

	// 가리키고 있는 Texture가 누구인지 저장
	SaveAssetRef(pFile, m_Atlas.Get());

	// UV 정보 저장
	fwrite(&m_LeftTopUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_SliceUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_BackgroundUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_OffsetUV, sizeof(Vec2), 1, pFile);

	fclose(pFile);

	return 0;
}

int ASprite::Load(const wstring& _FilePath)
{
	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	FILE_OPEN(_FilePath, L"rb");

	// 저장할때 가라키던 텍스쳐를 불러온다,
	m_Atlas = LoadAssetRef<ATexture>(pFile);

	// UV 정보 불러오기
	fread(&m_LeftTopUV, sizeof(Vec2), 1, pFile);
	fread(&m_SliceUV, sizeof(Vec2), 1, pFile);
	fread(&m_BackgroundUV, sizeof(Vec2), 1, pFile);
	fread(&m_OffsetUV, sizeof(Vec2), 1, pFile);

	fclose(pFile);

	return 0;
}