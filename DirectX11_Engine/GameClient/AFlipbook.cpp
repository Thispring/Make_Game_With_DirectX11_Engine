#include "pch.h"
#include "AFlipbook.h"
#include "AssetMgr.h"

AFlipbook::AFlipbook()
	: Asset(ASSET_TYPE::FLIPBOOK)
{
}

AFlipbook::~AFlipbook()
{
}

int AFlipbook::Save(const wstring& _FilePath)
{
	FILE_OPEN(_FilePath, L"wb");

	// 몇 장의 Sprite로 구성되었는지 저장
	UINT SpriteCount = (UINT)m_vecSprite.size();
	fwrite(&SpriteCount, sizeof(UINT), 1, pFile);

	// 가리키고 있었던 Sprite가 어떤건지를 파일에 저장
	for (const auto& Sprite : m_vecSprite)
		SaveAssetRef(pFile, Sprite.Get());

	fclose(pFile);

	return 0;
}

int AFlipbook::Load(const wstring& _FilePath)
{
	FILE_OPEN(_FilePath, L"rb");

	// 몇 장의 Sprite로 구성되었는지를 불러온다.
	UINT SpriteCount = 0;
	fread(&SpriteCount, sizeof(UINT), 1, pFile);

	// 가리키고 있었던 Sprite가 어떤건지를 파일에서 불러온다.
	for (UINT i = 0; i < SpriteCount; i++)
	{
		Ptr<ASprite> pSprite = LoadAssetRef<ASprite>(pFile);

		AddSprite(pSprite);
	}

	fclose(pFile);

	return 0;
}
