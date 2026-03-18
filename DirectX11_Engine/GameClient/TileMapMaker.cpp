#include "pch.h"
#include "TileMapMaker.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "imguiFunc.h"

TileMapMaker::TileMapMaker()
	: EditorUI("TileMapMaker")
	, m_vecSprite {}
	, m_Row(0)
	, m_Col(0)
	, m_TileSize {}
	, m_TileMapName {}
	, m_AtlasName {}
	, m_IsShowPreTile(false)
	, m_IsShowInit(false)
{

}

TileMapMaker::~TileMapMaker()
{

}


void TileMapMaker::ClearSetting()
{
	m_vecSprite = {};
	m_Row = 0;
	m_Col = 0;
	m_TileSize = Vec2(0.f, 0.f);
	m_TileMapName = {};
	m_AtlasName = {};
	m_IsShowPreTile = false;
	m_IsShowInit = false;
}

void TileMapMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making TileMap", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine(300.f);

	#pragma region SaveTileMapBtn
	if (ImGuiFunc::ColoredButton("SaveTileMap##TileMapSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("TileMapSave?");
	}

	// 모달 창을 매 프레임 마다 호출되게 하고,
	// 팝업 상태가 Open일 때, 실행된다.
	// 
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("TileMapSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// UINT 변환, 음수일 경우 크래시
			if (m_Row < 0 && m_Col < 0)
			{
				assert(false);
			}

			// 저장 및 초기화
			AssetMgr::GetInst()->CreateEngineTileMap(m_vecSprite, m_TileMapName, m_AtlasName, (UINT)m_Row, (UINT)m_Col, m_TileSize);
			ClearSetting();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	SPACING_UI(7);
	ImGui::Separator();
	#pragma endregion

	#pragma region Row
	OutputTitle("Row", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##ROW", &m_Row, 1.f, 0, INT_MAX))
	{
		SetRow(m_Row);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion
	
	#pragma region Col
	OutputTitle("Col", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.2f);
	if (ImGui::DragInt("##COL", &m_Col, 1.f, 0, INT_MAX))
	{
		SetCol(m_Col);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region TileSize
	OutputTitle("TileSize", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	if (ImGui::DragFloat2("##TILESIZE", m_TileSize, 1.f, 0.f, FLT_MAX))
	{
		SetTileSize(m_TileSize);
	}
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region TileMap PreView
	// Row, Col, TileSize를 받아
	// 더미 Tile UI를 생성합니다.
	// 위 정보를 입력받아 버튼을 누르면 더미 Tile 이미지 생성
	OutputTitle("TileMap PreSet", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	ImGui::Spacing();
	if (ImGui::Button("Create TileMap PreSet Button##TileMapPreSetBtn"))
	{
		// 부동 소수점 오차 고려
		const float EPSILON = 1e-6f;

		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		if (m_Row != 0 && m_Col != 0 && fabs(m_TileSize.x) > EPSILON && fabs(m_TileSize.y) > EPSILON)
			m_IsShowPreTile = true;
	}

	if (m_IsShowPreTile)
	{
		int Count = 0;
		//m_vecSprite.clear();

		// Init
		// 1번만 초기화, 이후 clear가 없어, Row, Col이 변할 시 예외상황에 대한 코드 작성하기
		if (!m_IsShowInit)
		{
			for (int i = 0; i < m_Col; ++i)
			{
				for (int j = 0; j < m_Row; ++j)
				{
					Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset(ASSET_TYPE::SPRITE, L"Sprite\\grid.sprite");
					Ptr<ASprite> pSprite = (ASprite*)pAsset.Get();
					m_vecSprite.push_back(pSprite);
				}
				Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset(ASSET_TYPE::SPRITE, L"Sprite\\grid.sprite");
				Ptr<ASprite> pSprite = (ASprite*)pAsset.Get();
				m_vecSprite.push_back(pSprite);
			}
			
			m_IsShowInit = true;
		}

		// TileSize가 보이는 것보다 작아서 보정치 *2
		for (int i = 0; i < m_Col; ++i) // 세로줄 반복
		{
			// 줄이 바뀔 때마다 ID 충돌 방지를 위한 Group ID
			ImGui::PushID(i);

			for (int j = 0; j < m_Row; ++j) // 가로줄 반복
			{
				// 첫 번째 열(j=0)이 아닐 때만 SameLine (옆으로 붙이기)
				if (j > 0) ImGui::SameLine();

				// 각 이미지마다 고유 ID 부여
				ImGui::PushID(j);

				// 인덱스 범위 체크 
				if (Count < m_vecSprite.size())
				{
					Vec2 LeftTopUV = m_vecSprite[Count]->GetLeftTopUV();
					Vec2 SliceUV = m_vecSprite[Count]->GetSliceUV();

					// 이미지 그리기
					ImGui::Image((ImTextureRef)m_vecSprite[Count]->GetAtlas()->GetSRV().Get(),m_TileSize * 2, LeftTopUV, LeftTopUV + SliceUV);

					// 드래그 앤 드롭 타겟 설정
					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
						if (PayLoad)
						{
							DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
							Ptr<Asset> pAsset = (Asset*)data;

							if (ASSET_TYPE::SPRITE == pAsset->GetType())
							{
								// 현재 Count 인덱스의 스프라이트만 교체
								m_vecSprite[Count] = ((ASprite*)pAsset.Get());
							}
						}
						ImGui::EndDragDropTarget();
					}
				}

				ImGui::PopID(); // 안쪽 ID Pop

				// 이미지 하나 처리 후 Count 증가
				Count++;
			}

			ImGui::PopID(); // 바깥쪽 ID Pop (Row ID)
		}
	}
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Atlas Name
	// Atlas Name
	OutputTitle("Atlas Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string atlasName = string(m_AtlasName.begin(), m_AtlasName.end());
	if (ImGui::InputTextWithHint("##ATLASTEXTURENAME", "Example: tile_Atlas", &atlasName))
	{
		wstring wAtlasName = wstring(atlasName.begin(), atlasName.end());
		SetAtlasName(wAtlasName);
	}

	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		/**************************************************************
		* 마우스가 때졌을 때의 조건, 전달한 Key값과 동일한지 확인
		*
		* 동작 의도에 맞게, 어느 ImGui에서 전달한 Key인지를 조건으로 구별하여
		* MeshRenderUI이면 ContentUI의 Mesh 목록에 있는 Key만을 받게 설계
		**************************************************************/
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			// 가져온 Texture의 Key 문자열을 세팅
			SetAtlasName(pAsset->GetKey());
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Drag and drop an image from the ContentUI\nTexture list into the box above.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region TileMap name to save
	OutputTitle("TileMap name to save", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string tileMapName = string(m_TileMapName.begin(), m_TileMapName.end());
	if (ImGui::InputTextWithHint("##TILEMAPNAMETOSAVE", "Name your TileMap", &tileMapName))
	{
		wstring wtileMapName = wstring(tileMapName.begin(), tileMapName.end());
		SetTileMapName(wtileMapName);
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
			"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

}
