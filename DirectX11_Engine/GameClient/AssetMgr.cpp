#include "pch.h"

#include "AMesh.h"
#include "AGraphicShader.h"
#include "ATexture.h"
#include "AMaterial.h"

#include "AssetMgr.h"
#include "PathMgr.h"

AssetMgr::AssetMgr()
	: m_Changed(false)
	, m_tempType()
	, m_vecFileName{}
	, m_fileCount(0)
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

void AssetMgr::LoadContent()
{
	wstring Path = {};	// 파일 경로
	string ext = {};	// 확장자

	// Type을 검사하기 위한 변수
	// MESH 부터 시작하여 검사가 끝나면 +1로 하여
	// END까지 에셋이 있는지를 검사합니다.
	m_tempType = ASSET_TYPE::MESH;
	m_vecFileName = {};
	m_fileCount = 0;


	while (true)
	{
		// CONTENT_PATH + "Asset 폴더 이름" 으로 경로 지정


		// ASSET_TYPE을 조건으로
		// Path와 ext를 설정합니다.
		// 
		// Asset 파일로 만들지 않은 Type의 경우, 일단 주석으로 생략
		switch (m_tempType)
		{
		//case ASSET_TYPE::MESH:
		//	break;
		case ASSET_TYPE::MATERIAL:
			Path = (wstring)CONTENT_PATH + L"Material\\";
			ext = ".mtrl";
			break;
		// Texture는 AssetMgr Init에서 Load 진행
		//case ASSET_TYPE::TEXTURE:
		//	break;
		//case ASSET_TYPE::SOUND:
		//	break;
		//case ASSET_TYPE::GRAPHICSHADER:
		//	break;
		//case ASSET_TYPE::COMPUTESHADER:
		//	break;
		// 
		// Level의 경우 생성 시점을 고려(Level에 있는 Object들중 RenderComponent가, Asset Loading 전에 불려지고 있음)
		case ASSET_TYPE::LEVEL:
			Path = (wstring)CONTENT_PATH + L"Level\\";
			ext = ".lv";
			break;
		case ASSET_TYPE::SPRITE:
			Path = (wstring)CONTENT_PATH + L"Sprite\\";
			ext = ".sprite";
			break;
		case ASSET_TYPE::FLIPBOOK:
			Path = (wstring)CONTENT_PATH + L"Flipbook\\";
			ext = ".flip";
			break;
		case ASSET_TYPE::TILEMAP:
			Path = (wstring)CONTENT_PATH + L"TileMap\\";
			ext = ".tile";
			break;
		//case ASSET_TYPE::PREFAB:
		//	break;
		case ASSET_TYPE::END:
			
			break;
		}

		// END까지 왔다면 반복 종료
		if (m_tempType == ASSET_TYPE::END)
			break;

		// 해당 경로가 존재하는지, 그리고 디렉토리인지 확인
		if (fs::exists(Path) && fs::is_directory(Path))
		{
			for (const auto& entry : fs::directory_iterator(Path))
			{
				// 일반 파일이고 확장자가 일치하는지 확인
				if (entry.is_regular_file() && entry.path().extension() == ext)
				{
					m_vecFileName.push_back(entry.path().filename().wstring());
					m_fileCount++;
				}
			}
		}

		// 이름과 확장자 타입을 가져와 LOAD 매크로 매개변수로 전달
		
		switch (m_tempType)
		{
			//case ASSET_TYPE::MESH:
			//	break;
		case ASSET_TYPE::MATERIAL:
			for (auto& FilePath : m_vecFileName)
			{
				Load<AMaterial>(L"Material\\" + FilePath, L"Material\\" + FilePath);
			}
			break;
			// Texture는 AssetMgr Init에서 Load 진행
			//case ASSET_TYPE::TEXTURE:
			//	break;
			//case ASSET_TYPE::SOUND:
			//	break;
			//case ASSET_TYPE::GRAPHICSHADER:
			//	break;
			//case ASSET_TYPE::COMPUTESHADER:
			//	break;
		case ASSET_TYPE::SPRITE:
			for (auto& FilePath : m_vecFileName)
			{
				Load<ASprite>(L"Sprite\\" + FilePath, L"Sprite\\" + FilePath);
			}
			break;
		case ASSET_TYPE::FLIPBOOK:
			for (auto& FilePath : m_vecFileName)
			{
				Load<AFlipbook>(L"Flipbook\\" + FilePath, L"Flipbook\\" + FilePath);
			}
			break;
		case ASSET_TYPE::TILEMAP:
			for (auto& FilePath : m_vecFileName)
			{
				Load<ATileMap>(L"TileMap\\" + FilePath, L"TileMap\\" + FilePath);
			}
			break;
			//case ASSET_TYPE::PREFAB:
			//	break;
		
		// Level은 나머지 Asset들이 모두 Load된 후, 마지막에 호출
		case ASSET_TYPE::LEVEL:
			for (auto& FilePath : m_vecFileName)
			{
				Load<ALevel>(L"Level\\" + FilePath, L"Level\\" + FilePath);
				// func ChangeLevel 함수 호출
				ChangeLevel(L"Level\\" + FilePath);
			}
			break;
		}

		// 검사와 Load가 모두 끝나면 ASSET_TYPE +1 연산으로 다음 TYPE을 가리키게 합니다.
		m_tempType = static_cast<ASSET_TYPE>(static_cast<int>(m_tempType) + 1);
		// vector 초기화
		m_vecFileName.clear();
	}
	
	// Level은 나머지 Asset들이 모두 Load된 후, 마지막에 호출
	//switch (m_tempType)
	//{
	//case ASSET_TYPE::LEVEL:
	//	for (auto& FilePath : m_vecFileName)
	//	{
	//		Load<ALevel>(L"Level\\" + FilePath, L"Level\\" + FilePath);
	//	}
	//	break;
	//}

	// LevelMgr에서 시작할 Level Load
	//Ptr<ALevel> pLevel = LOAD(ALevel, L"Level\\Normal_Stage_0.lv");
	// func ChangeLevel 함수 호출
	//ChangeLevel(L"Level\\Normal_Stage_0.lv");
}

void AssetMgr::GetAssetNames(ASSET_TYPE _type, vector<wstring>& _vec)
{
	for (const auto& pair : m_mapAsset[(UINT)_type])
	{
		_vec.push_back(pair.first);
	}
}