#include "pch.h"
#include "AssetMgr.h"
#include "PathMgr.h"

void AssetMgr::Init() 
{
	CreateEngineMesh();

	CreateEngineShader();

	CreateEngineTexture();

	//CreateEngineMaterial();

	//CreateEngineSprite();

	CreateEnginePrefab();
}

void AssetMgr::CreateEngineMesh()
{
	/*************************************
	* 정점 순서는 좌측최상단 부터, 시작 
	* 왼쪽 -> 오른쪽 -> 다음줄 왼쪽 -> ...
	* 내려가면서 순차적으로 작성
	* 
	* 정점 좌표에 따라 UV 좌표 설정
	*************************************/

	//============
	// 정사각형 매쉬 
	//============
	Ptr<AMesh> pSquareMesh = nullptr;
	Vtx squarVtx[4] = {};

	squarVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	squarVtx[0].vUV = Vec2(0.f, 0.f);
	squarVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 0.f);

	squarVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	squarVtx[1].vUV = Vec2(1.f, 0.f);
	squarVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 0.f);

	squarVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	squarVtx[2].vUV = Vec2(1.f, 1.f);
	squarVtx[2].vColor = Vec4(0.f, 1.f, 0.f, 0.f);

	squarVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	squarVtx[3].vUV = Vec2(0.f, 1.f);
	squarVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 0.f);

	// 인덱스 버퍼
	UINT arrSquareIdx[6] = { 0, 2, 3, 0, 1, 2 };

	// g_RectMesh 포인터를 활용해서, 동적할당
	// 사각형 매쉬 생성
	pSquareMesh = new AMesh;
	pSquareMesh->Create(squarVtx, 4, arrSquareIdx, 6);

	// 에셋 매니저 등록
	AddAsset(L"SquareMesh", pSquareMesh.Get());


	// ====================
	// SquareMesh_LineStrip
	// ====================
	arrSquareIdx[0] = 0; 	arrSquareIdx[1] = 1;	arrSquareIdx[2] = 2;	arrSquareIdx[3] = 3; arrSquareIdx[4] = 0;
	pSquareMesh = new AMesh;
	pSquareMesh->Create(squarVtx, 4, arrSquareIdx, 5);
	AddAsset(L"SquareMesh_LineStrip", pSquareMesh.Get());


	//============
	// 직사각형 매쉬
	//============
	Ptr<AMesh> pRectMesh = nullptr;
	Vtx rectVtx[4] = {};

	rectVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	rectVtx[0].vUV = Vec2(0.f, 0.f);
	rectVtx[0].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	rectVtx[1].vPos = Vec3(0.25f, 0.5f, 0.f);
	rectVtx[1].vUV = Vec2(1.f, 0.f);
	rectVtx[1].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	rectVtx[2].vPos = Vec3(0.25f, -0.5f, 0.f);
	rectVtx[2].vUV = Vec2(1.f, 1.f);
	rectVtx[2].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	rectVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	rectVtx[3].vUV = Vec2(0.f, 1.f);
	rectVtx[3].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	UINT arrRectIdx[6] = { 0, 2, 3, 0, 1, 2 };
	pRectMesh = new AMesh;
	pRectMesh->Create(rectVtx, 4, arrRectIdx, 6);
	AddAsset(L"RectMesh", pRectMesh.Get());


	// ==================
	// RectMesh_LineStrip
	// ==================
	arrRectIdx[0] = 0; 	arrRectIdx[1] = 1;	arrRectIdx[2] = 2;	arrRectIdx[3] = 3; arrRectIdx[4] = 0;
	pRectMesh = new AMesh;
	pRectMesh->Create(rectVtx, 4, arrRectIdx, 5);
	AddAsset(L"RectMesh_LineStrip", pRectMesh.Get());


	// ===========================
	// 직사각형 정점 (Full), 배경활용
	// ===========================
	Ptr<AMesh> pFRectMesh = nullptr;
	Vtx FRectVtx[4] = {};

	FRectVtx[0].vPos = Vec3(-1.f, 1.f, 0.f);
	FRectVtx[0].vUV = Vec2(0.f, 0.f);
	FRectVtx[0].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	FRectVtx[1].vPos = Vec3(1.f, 1.f, 0.f);
	FRectVtx[1].vUV = Vec2(1.f, 0.f);
	FRectVtx[1].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	FRectVtx[2].vPos = Vec3(1.f, -1.f, 0.f);
	FRectVtx[2].vUV = Vec2(1.f, 1.f);
	FRectVtx[2].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	FRectVtx[3].vPos = Vec3(-1.f, -1.f, 0.f);
	FRectVtx[3].vUV = Vec2(0.f, 1.f);
	FRectVtx[3].vColor = Vec4(0.f, 0.f, 0.f, 0.f);

	pFRectMesh = new AMesh;
	pFRectMesh->Create(FRectVtx, 4, arrRectIdx, 6);
	AddAsset(L"FullRectMesh", pFRectMesh.Get());


	//=========
	// 원 만들기
	//=========
	/***************************************************
	* 원점을 기준으로 설정한 반지름까지의 하나의 정점을
	* 해장 정점을 360도 회전할때의 회전 경로에 수많은 정점을 
	* 찍어 연결한다면, 원처럼 표현이 가능합니다.
	***************************************************/
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	// 중점
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Theta = 0.f;
	float Radius = 0.5f;
	float Slice = 20.f;	// 정점의 개수 결정

	// 원의 테두리 정점 추가
	for (int i = 0; i < (int)Slice + 1; i++)
	{
		v.vPos = Vec3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		//v.vUV = Vec2(0.5f, 0.5f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Theta += XM_2PI / Slice;
	}

	// 인덱스
	for (int i = 0; i < (int)Slice + 1; i++)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 매쉬 생성
	Ptr<AMesh> pMesh = nullptr;
	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh", pMesh.Get());


	// ====================
	// CircleMesh_LineStrip
	// ====================
	vecIdx.clear();
	for (int i = 0; i < (int)Slice + 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh_LineStrip", pMesh.Get());

}

void AssetMgr::CreateEngineShader() 
{
	// ===========================
	// 기본 2D 파이프라인(쉐이더) 생성
	// ===========================
	Ptr<AGraphicShader> pShader = nullptr;

	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->AddShaderParam(SHADER_PARAM::VEC4, 0, L"TintColor");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");
	
	AddAsset(L"Std2DShader", pShader.Get());


	// =====================
	// 2D ONE_ONE 쉐이더 생성
	// =====================
	pShader = nullptr;
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	AddAsset(L"OOStd2DShader", pShader.Get());


	//=================
	// DbgRenderShader
	// 디버그 쉐이더 추가
	//=================
	pShader = nullptr;
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"Shader\\dbg.fx", "VS_Debug");
	pShader->CreatePixelShader(L"Shader\\dbg.fx", "PS_Debug");
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);	// 토폴로지 설정
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);				// Debug Shader 타입
	pShader->SetBSType(BS_TYPE::DEFAULT);
	AddAsset(L"DbgShader", pShader.Get());


	/****************************************************
	* (26-02-26): 각각의 RenderComponent에서 자신이 사용할
	* 전용 쉐이더를 생성하는 부분을 Init에서 진행
	****************************************************/ 
	// ===============
	// BillboardShader
	// ===============
	pShader = new AGraphicShader;
	pShader->SetName(L"BillboardShader");
	pShader->CreateVertexShader(L"Shader\\billboard.fx", "VS_Billboard");
	pShader->CreatePixelShader(L"Shader\\billboard.fx", "PS_Billboard");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());


	// ============
	// SpriteShader
	// ============
	pShader = new AGraphicShader;
	pShader->SetName(L"SpriteShader");
	pShader->CreateVertexShader(L"Shader\\sprite.fx", "VS_Sprite");
	pShader->CreatePixelShader(L"Shader\\sprite.fx", "PS_Sprite");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());


	// ==============
	// FlipbookShader
	// ==============
	pShader = new AGraphicShader;
	pShader->SetName(L"FlipbookShader");
	pShader->CreateVertexShader(L"Shader\\flipbook.fx", "VS_Flipbook");
	pShader->CreatePixelShader(L"Shader\\flipbook.fx", "PS_Flipbook");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());


	// =============
	// TileMapShader
	// =============
	pShader = new AGraphicShader;
	pShader->SetName(L"TileShader");
	pShader->CreateVertexShader(L"Shader\\tile.fx", "VS_Tile");
	pShader->CreatePixelShader(L"Shader\\tile.fx", "PS_Tile");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());
}

void AssetMgr::CreateEngineTexture() 
{
	// Default Texture
	Load<ATexture>(L"cubeImg", L"Texture\\cube.png");
	
	// Player Character Atlas
	Load<ATexture>(L"cat1_Atlas", L"Texture\\Player\\cat1_base.png");
	Load<ATexture>(L"cat2_Atlas", L"Texture\\Player\\cat2_base.png");
	Load<ATexture>(L"catAR_Atlas", L"Texture\\Player\\cat_ar_base.png");
	Load<ATexture>(L"energyFX_Atlas", L"Texture\\Player\\energy_effect_base.png");

	// Enemy Atlas
	Load<ATexture>(L"mon1_Atlas", L"Texture\\Enemy\\mon1_sprite_base.png");
	Load<ATexture>(L"mon2_Atlas", L"Texture\\Enemy\\mon2_sprite_base.png");
	Load<ATexture>(L"mon3_Atlas", L"Texture\\Enemy\\mon3_sprite_base.png");
	Load<ATexture>(L"mon4_Atlas", L"Texture\\Enemy\\mon4_sprite_base.png");

	Load<ATexture>(L"boss1_Atlas", L"Texture\\Enemy\\mechaneko-sheet1-r3-alpha.png");
	Load<ATexture>(L"boss2_Atlas", L"Texture\\Enemy\\mechaneko-sheet2-r1-alpha.png");
	
	Load<ATexture>(L"bomb_Atlas", L"Texture\\Enemy\\pirate_bomb_image_sequence.png");

	// BackGround
	Load<ATexture>(L"cloudImg", L"Texture\\BackGround\\clouds.png");
	Load<ATexture>(L"seaImg", L"Texture\\BackGround\\sea.png");
	Load<ATexture>(L"skyImg", L"Texture\\BackGround\\sky.png");
	Load<ATexture>(L"groundImg", L"Texture\\BackGround\\far-grounds.png");

	// TileMap Atlas
	Load<ATexture>(L"tile_Atlas", L"Texture\\BackGround\\tileset.png");

	// UI

}

void AssetMgr::CreateEngineMaterial() 
{

}

void AssetMgr::CreateEngineSprite()
{
	/***********************************************
	* NOTE(26-02-27): Sprite를 파일로 저장하기 위해
	* 생성 시, 경로 + 확장자 명을 문자열로 추가합니다.
	* 해당 문자열은 생성된 Sprite의 Key값으로 설정 합니다.
	***********************************************/
}

void AssetMgr::CreateEnginePrefab()
{
	//// Missile 역할 프리팹 제작
	//GameObject* pObject = new GameObject;
	//pObject->SetName(L"Missile");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CCollider2D);	
	//pObject->AddComponent(new CMissileScript);		

	//pObject->Transform()->SetRelativeScale(Vec3(10.f, 30.f, 1.f));

	//pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"Std2DMtrl"));

	//// 미사일 오브젝트를 프리팹으로 등록
	//Ptr<APrefab> pMissilePrefab = new APrefab;
	//pMissilePrefab->SetObject(pObject);
	//AddAsset(L"Prefab\\Missile.pref", pMissilePrefab.Get());

	//wstring FilePath = CONTENT_PATH + L"Prefab\\Missile.pref";
	//pMissilePrefab->Save(FilePath);
}

void AssetMgr::CreateEngineSprite(wstring _Name, Vec2 _Slice, int _StartLoop, int _EndLoop, wstring _Path, int _OriIdx)
{
	#pragma region Atlas 이미지에서 Texture를 생성하는 방법
	Ptr<ATexture> pAtlas = FIND(ATexture, _Name);		// Texture 생성 시 설정한 이름 String을 입력합니다. 

	// Texture의 가로 세로 길이를 Getter로 얻어옵니다.
	float Width = pAtlas->GetWidth();
	float Height = pAtlas->GetHeight();

	// Texture 이미지를 얼마만큼의 길이로 자를 것인지 설정합니다.
	// UV 좌표 기준으로 값을 설정합니다. (자를 길이(높이, 너비)를 Vec2 값에 설정) 
	// 매개변수: _Slice 사용

	// 위에서 설정한 값과 반복문을 이용하여
	// Texture 이미지를 잘라 Sprite를 생성합니다. 
	Ptr<ASprite> pSprite = nullptr;
	for (int i = _StartLoop; i < _EndLoop; ++i)	// 몇 번 반복할지는, 몇 개의 이미지를 만들지에 맞게 결정합니다.
	{
		// Sprite의 이름을 설정합니다.
		// 만약 파일 형식으로 제작을 원한다면, 폴더 경로 + 확장자명을 적어둡니다.
		wchar_t Buff[50] = {};

		//==================================================================
		// NOTE(26-03-09): _Path 매개변수로 받았을 때, %d를 어떻게 합칠지 고민하기
		//==================================================================
		// _Path는 0%d 이전까지만 받고, 이후는 함수에서 +연산으로 붙이게 하기
		// std::to_wstring(i) 로 붙이는 방법이 있음
		// 10 이하이면 "0%d"로 네이밍 ex) 02.sprite
		wstring spritePath = {};

		if (i < 10) spritePath = _Path + to_wstring(0) + to_wstring(i) + L".sprite";
		else spritePath = _Path + to_wstring(i) + L".sprite";

		pSprite = new ASprite;
		pSprite->SetName(spritePath);
		pSprite->SetAtlas(pAtlas);

		// UV 기준 원점 설정
		// 원점 공식:
		// 파라미터로 받은 인덱스 * (_Slice.y / Height)
		float Origin = _OriIdx * (_Slice.y / Height);
		pSprite->SetLeftTopUV(Vec2((_Slice.x / Width) * (float)i, Origin));

		// x, y축 각각 '자를 해상도'에서 '아틀라스 해상도'를 나누기
		pSprite->SetSliceUV(_Slice / Vec2(Width, Height));

		// BackgorundUV는 자를 해상도(UV 기준 정규화) * 2로 설정 
		//==================================================================
		// NOTE(26-03-09): _Slice 매개변수를 UV 기준 정규화하는 기능 구현하기
		//==================================================================
		Vec2 bgUV = {((_Slice.x / Width) * 2), ((_Slice.y / Height) * 2)};
		pSprite->SetBackgroundUV(bgUV);

		// 만들어진 Sprite를 등록합니다.
		// 
		// AddAsset => 런타임에만 등록
		AddAsset(pSprite->GetName(), pSprite.Get());
		// Save => 파일 형태로 등록
		pSprite->Save(CONTENT_PATH + pSprite->GetKey());	// 경로가 곧 Key 값
	}
	#pragma endregion
}

void AssetMgr::CreateEngineFlipbook(wstring _SpriteName, wstring _FlipbookName, int _Loop)
{
	#pragma region Flipbook 생성하는 방법
	// Flipbook 객체 생성 후, 이름을 설정하고
	// 반복문을 통해, 이어서 보여줄 sprite들을 가르키게 합니다.
	Ptr<AFlipbook> pFlipbook = nullptr;

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(_FlipbookName);

	for (int i = 0; i < _Loop; ++i)
	{
		wstring spritePath = {};

		if (i < 10) spritePath = _SpriteName + to_wstring(0) + to_wstring(i) + L".sprite";
		else spritePath = _SpriteName + to_wstring(i) + L".sprite";

		pFlipbook->AddSprite(LOAD(ASprite, spritePath));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());
	// Save => 파일 형태로 등록
	pFlipbook->Save(CONTENT_PATH + pFlipbook->GetKey());	// 경로가 곧 Key 값
	#pragma endregion
}

void AssetMgr::CreateAssetByCode()
{
	#pragma region Mesh 생성하는 방법

	#pragma endregion

	#pragma region Shader 생성하는 방법

	#pragma endregion

	#pragma region Texture 생성하는 방법

	#pragma endregion

	#pragma region Material 생성하는 방법
	// File로 저장이 완료된 Asset은 Load 함수 호출부만 남겨두기

	wstring FilePath = CONTENT_PATH;
	Ptr<AMaterial> pMtrl = nullptr;

	// =========
	// Std2DMtrl 
	// =========
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Material\\Std2DMtrl.mtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"cubeImg"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(FilePath + pMtrl->GetKey());

	// =====
	// 디버그 
	// =====
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Material\\DbgMtrl.mtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"DbgShader"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_DEBUG);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(FilePath + pMtrl->GetKey());

	Load<AMaterial>(L"Material\\Std2DMtrl.mtrl", L"Material\\Std2DMtrl.mtrl");
	Load<AMaterial>(L"Material\\DbgMtrl.mtrl", L"Material\\DbgMtrl.mtrl");
	#pragma endregion

	#pragma region Atlas 이미지에서 Texture를 생성하는 방법
	Ptr<ATexture> pAtlas = FIND(ATexture, L"TestImage");		// Texture 생성 시 설정한 이름 String을 입력합니다. 
	
	// Texture의 가로 세로 길이를 Getter로 얻어옵니다.
	float Width = pAtlas->GetWidth();
	float Height = pAtlas->GetHeight();

	// Texture 이미지를 얼마만큼의 길이로 자를 것인지 설정합니다.
	// UV 좌표 기준으로 값을 설정합니다. (자를 길이(높이, 너비)를 Vec2 값에 설정) 
	Vec2 SlicePixel = Vec2(1.f, 1.f);

	// 위에서 설정한 값과 반복문을 이용하여
	// Texture 이미지를 잘라 Sprite를 생성합니다. 
	Ptr<ASprite> pSprite = nullptr;
	for (int i = 0; i < 10; ++i)	// 몇 번 반복할지는, 몇 개의 이미지를 만들지에 맞게 결정합니다.
	{
		// Sprite의 이름을 설정합니다.
		// 만약 파일 형식으로 제작을 원한다면, 폴더 경로 + 확장자명을 적어둡니다.
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\Test_Spirte_%d.sprite", i);		

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);

		// 원점 설정
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, 0.f));

		// x, y축 각각 '자를 해상도'에서 '아틀라스 해상도'를 나누기
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		
		// 만들어진 Sprite를 등록합니다.
		// 
		// AddAsset => 런타임에만 등록
		AddAsset(pSprite->GetName(), pSprite.Get());
		// Save => 파일 형태로 등록
		//pSprite->Save(CONTENT_PATH + pSprite->GetName());
		pSprite->Save(CONTENT_PATH + pSprite->GetKey());	// 경로가 곧 Key 값
	}
	#pragma endregion

	#pragma region Flipbook 생성하는 방법
	// Flipbook 객체 생성 후, 이름을 설정하고
	// 반복문을 통해, 이어서 보여줄 sprite들을 가리키게 합니다.
	Ptr<AFlipbook> pFlipbook = nullptr;

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\Test_Fb.flip");

	// 해당 Flipbook에 몇개의 Sprite를 연속으로 재생할것인지를
	// 반복 횟수로 정합니다. ex)8장이면 8번 반복
	for (int i = 0; i < 8; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\Test_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());
	// Save => 파일 형태로 등록
	pFlipbook->Save(CONTENT_PATH + pFlipbook->GetKey());
	#pragma endregion

	#pragma region TileMap 생성하는 방법
	// =======
	// TileMap
	// =======
	Ptr<ATileMap> pTileMap = nullptr;

	pTileMap = new ATileMap;
	pTileMap->SetName(L"TileMap\\TestTileMap.tile");
	pTileMap->SetRowCol(2, 2);
	pTileMap->SetTileSize(Vec2(16.f, 16.f));
	pTileMap->SetAtlas(FIND(ATexture, L"TileAtlas"));

	int Count = 0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j, ++Count)
		{
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Sprite\\TileSprite_%d.sprite", Count);
			pTileMap->SetSprite(i, j, FIND(ASprite, Buff));
		}
	}

	AddAsset(pTileMap->GetName(), pTileMap.Get());
	pTileMap->Save(pTileMap->GetKey());
	#pragma endregion
}
