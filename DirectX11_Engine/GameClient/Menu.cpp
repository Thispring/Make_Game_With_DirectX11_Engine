#include "pch.h"
#include "Menu.h"
#include "AssetMgr.h"
#include "EditorMgr.h"
#include "LevelMgr.h"
#include "ContentUI.h"


Menu::Menu()
	: EditorUI("Menu")
	, m_vecWStrLevel{}
	, m_vecLevelName{}
{
}

Menu::~Menu()
{
}

void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		File();
		
		Level();

		View();

		GameObject();

		Asset();

		ImGui::EndMainMenuBar();
	}
}

void Menu::Tick_UI()
{
}

void Menu::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{
			// Level의 변경점 저장, 기존에 만들었던 Level 저장 버튼 활용
			// 현재 Level의 변경점을 저장
			Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
			ChangeLevel(pLevel->GetKey());
		}

		if (ImGui::BeginMenu("Level Load"))
		{
			// Level Asset List를 보여주고, 선택한 Level을 불러오기
			// List를 누르면, 해당 Level의 문자열을 func.cpp의 ChangeLevel에 매개변수로 전달
			
			// MenuItem을 눌렀다면, 프로젝트에 등록된 모든 Level Type 에셋의 이름을 등록
			m_vecWStrLevel.clear();
			m_vecLevelName.clear();

			AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::LEVEL, m_vecWStrLevel);
			
			for (UINT i = 0; i < m_vecWStrLevel.size(); ++i)
			{
				m_vecLevelName.push_back(string(m_vecWStrLevel[i].begin(), m_vecWStrLevel[i].end()));

				// Level 이름을 가져와 MenuItem 생성
				if (ImGui::MenuItem(m_vecLevelName[i].c_str()))
				{
					// TaskMgr에게 다음 프레임에 실행할 Level을 변경하도록 요청
					// wstring과 string 벡터는 같은 index에 같은 데이터를 보장해야 합니다.
					ChangeLevel(m_vecWStrLevel[i]);
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void Menu::Level()
{
	if (ImGui::BeginMenu("Level"))
	{
		bool HasLevel = LevelMgr::GetInst()->GetCurLevel().Get();
		bool IsPlay = false, IsPause = false, IsStop = false;

		if (HasLevel)
		{
			// 현재 Level의 상태를 읽어와, 알맞은 변수 상태 설정
			LEVEL_STATE CurState = LevelMgr::GetInst()->GetLevelState();
			if (LEVEL_STATE::PLAY == CurState)
				IsPlay = true;
			else if (LEVEL_STATE::PAUSE == CurState)
				IsPause = true;
			else if (LEVEL_STATE::STOP == CurState)
				IsStop = true;
		}

		// Flag에 따라 ImGui 활성/비활성화
		if (ImGui::MenuItem("Play", nullptr, nullptr, HasLevel && !IsPlay))
			ChangeLevelState(LEVEL_STATE::PLAY);

		if (ImGui::MenuItem("Pause", nullptr, nullptr, HasLevel && IsPlay))
			ChangeLevelState(LEVEL_STATE::PAUSE);

		if (ImGui::MenuItem("Stop", nullptr, nullptr, HasLevel && !IsStop))
			ChangeLevelState(LEVEL_STATE::STOP);

		ImGui::EndMenu();
	}
}

void Menu::View()
{
	if (ImGui::BeginMenu("View"))
	{
		/******************************
		* 새로운 ImGui 창은 여기에서 생성
		******************************/

		bool ShowDemo = EditorMgr::GetInst()->IsShowDemo();
		if (ImGui::MenuItem("Demo", nullptr, &ShowDemo, true))
		{
			EditorMgr::GetInst()->ShowDemo(ShowDemo);
		}

		Ptr<EditorUI> pInspector = EditorMgr::GetInst()->FindUI("Inspector");
		bool InspectorActive = pInspector->IsActive();
		if (ImGui::MenuItem("Inspector", nullptr, &InspectorActive))
		{
			pInspector->SetActive(InspectorActive);
		}

		Ptr<EditorUI> pOutliner = EditorMgr::GetInst()->FindUI("Outliner");
		bool OutlinerActive = pOutliner->IsActive();
		if (ImGui::MenuItem("Outliner", nullptr, &OutlinerActive))
		{
			pOutliner->SetActive(OutlinerActive);
		}

		Ptr<EditorUI> pKeyList = EditorMgr::GetInst()->FindUI("KeyList");
		bool KeyListActive = pKeyList->IsActive();
		if (ImGui::MenuItem("KeyList", nullptr, &KeyListActive))
		{
			pKeyList->SetActive(KeyListActive);
		}

		Ptr<EditorUI> pSpriteMaker = EditorMgr::GetInst()->FindUI("SpriteMaker");
		bool SpriteMakerActive = pSpriteMaker->IsActive();
		if (ImGui::MenuItem("SpriteMaker", nullptr, &SpriteMakerActive))
		{
			pSpriteMaker->SetActive(SpriteMakerActive);
		}

		Ptr<EditorUI> pFlipbookMaker = EditorMgr::GetInst()->FindUI("FlipbookMaker");
		bool FlipbookMakerActive = pFlipbookMaker->IsActive();
		if (ImGui::MenuItem("FlipbookMaker", nullptr, &FlipbookMakerActive))
		{
			pFlipbookMaker->SetActive(FlipbookMakerActive);
		}

		Ptr<EditorUI> pTileMapMaker = EditorMgr::GetInst()->FindUI("TileMapMaker");
		bool TileMapMakerActive = pTileMapMaker->IsActive();
		if (ImGui::MenuItem("TileMapMaker", nullptr, &TileMapMakerActive))
		{
			pTileMapMaker->SetActive(TileMapMakerActive);
		}

		Ptr<EditorUI> pMtrlMaker = EditorMgr::GetInst()->FindUI("MaterialMaker");
		bool MtrlMakerActive = pMtrlMaker->IsActive();
		if (ImGui::MenuItem("MaterialMaker", nullptr, &MtrlMakerActive))
		{
			pMtrlMaker->SetActive(MtrlMakerActive);
		}

		Ptr<EditorUI> pGameObjMaker = EditorMgr::GetInst()->FindUI("GameObjectMaker");
		bool GameObjMakerActive = pGameObjMaker->IsActive();
		if (ImGui::MenuItem("GameObjectMaker", nullptr, &GameObjMakerActive))
		{
			pGameObjMaker->SetActive(GameObjMakerActive);
		}

		Ptr<EditorUI> pPrefabMaker = EditorMgr::GetInst()->FindUI("PrefabMaker");
		bool PrefabMakerActive = pPrefabMaker->IsActive();
		if (ImGui::MenuItem("PrefabMaker", nullptr, &PrefabMakerActive))
		{
			pPrefabMaker->SetActive(PrefabMakerActive);
		}

		Ptr<EditorUI> pLevelMaker = EditorMgr::GetInst()->FindUI("LevelMaker");
		bool LevelMakerActive = pLevelMaker->IsActive();
		if (ImGui::MenuItem("LevelMaker", nullptr, &LevelMakerActive))
		{
			pLevelMaker->SetActive(LevelMakerActive);
		}

		ImGui::EndMenu();
	}
}

void Menu::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		Ptr<EditorUI> pGameObjMaker = EditorMgr::GetInst()->FindUI("GameObjectMaker");
		bool GameObjMakerActive = pGameObjMaker->IsActive();
		if (ImGui::MenuItem("Create GameObject", nullptr, &GameObjMakerActive))
		{
			pGameObjMaker->SetActive(GameObjMakerActive);
		}

		ImGui::EndMenu();
	}
}

void Menu::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::BeginMenu("Create Asset"))
		{
			Ptr<EditorUI> pMtrlMaker = EditorMgr::GetInst()->FindUI("MaterialMaker");
			bool MtrlMakerActive = pMtrlMaker->IsActive();
			if (ImGui::MenuItem("Create Material", nullptr, &MtrlMakerActive))
			{
				pMtrlMaker->SetActive(MtrlMakerActive);
			}
	
			// 추후 Create Sprite, Flipbook, TileMap 클래스 분리
			Ptr<EditorUI> pSpriteMaker = EditorMgr::GetInst()->FindUI("SpriteMaker");
			bool SpriteMakerActive = pSpriteMaker->IsActive();
			if (ImGui::MenuItem("Create Sprite", nullptr, &SpriteMakerActive))
			{
				pSpriteMaker->SetActive(SpriteMakerActive);
			}

			Ptr<EditorUI> pFlipbookMaker = EditorMgr::GetInst()->FindUI("FlipbookMaker");
			bool FlipbookMakerActive = pFlipbookMaker->IsActive();
			if (ImGui::MenuItem("Create Flipbook", nullptr, &FlipbookMakerActive))
			{
				pFlipbookMaker->SetActive(FlipbookMakerActive);
			}

			Ptr<EditorUI> pTileMapMaker = EditorMgr::GetInst()->FindUI("TileMapMaker");
			bool TileMapMakerActive = pTileMapMaker->IsActive();
			if (ImGui::MenuItem("Create TileMap", nullptr, &TileMapMakerActive))
			{
				pTileMapMaker->SetActive(TileMapMakerActive);
			}

			Ptr<EditorUI> pPrefabMaker = EditorMgr::GetInst()->FindUI("PrefabMaker");
			bool PrefabMakerActive = pPrefabMaker->IsActive();
			if (ImGui::MenuItem("Create Prefab", nullptr, &PrefabMakerActive))
			{
				pPrefabMaker->SetActive(PrefabMakerActive);
			}

			Ptr<EditorUI> pLevelMaker = EditorMgr::GetInst()->FindUI("LevelMaker");
			bool LevelMakerActive = pLevelMaker->IsActive();
			if (ImGui::MenuItem("Create Level", nullptr, &LevelMakerActive))
			{
				pLevelMaker->SetActive(LevelMakerActive);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}


wstring Menu::GetAssetName(ASSET_TYPE _Type, const wstring& _Name)
{
	wstring Ext;

	switch (_Type)
	{
	case ASSET_TYPE::MESH:
		Ext = L".mesh";
		break;
	case ASSET_TYPE::MATERIAL:
		Ext = L".mtrl";
		break;
	case ASSET_TYPE::TEXTURE:
	case ASSET_TYPE::SOUND:
	case ASSET_TYPE::GRAPHICSHADER:
	case ASSET_TYPE::COMPUTESHADER:
		assert(nullptr);
		break;
	case ASSET_TYPE::SPRITE:
		Ext = L".sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		Ext = L".flip";
		break;
	case ASSET_TYPE::PREFAB:
		Ext = L".pref";
		break;
	case ASSET_TYPE::LEVEL:
		Ext = L".lv";
		break;
	}

	int i = 0;
	while (true)
	{
		wchar_t Num[50] = {};
		swprintf_s(Num, 50, L"_%d", i);

		wstring AssetName = wstring(_Name + Num + Ext);
		if (nullptr == AssetMgr::GetInst()->FindAsset(_Type, AssetName))
		{
			return AssetName;
		}

		i++;
	}
}