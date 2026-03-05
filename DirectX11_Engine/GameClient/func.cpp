#include "pch.h"
#include "RenderMgr.h"
#include "TaskMgr.h"

void CreateObject(GameObject* _Object, int LayerIdx)
{
    TaskInfo info = {};

    info.Type = TASK_TYPE::CREATE_OBJECT;
    info.Param_0 = (DWORD_PTR)_Object;
    info.Param_1 = LayerIdx;

    TaskMgr::GetInst()->AddTask(info);
}

wchar_t Buff[255] = {};
void ChangeLevel(const wstring& _NextLevelName)
{
	TaskInfo info = {};

	wcscpy_s(Buff, 255, _NextLevelName.c_str());

	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param_0 = (DWORD_PTR)Buff;

	TaskMgr::GetInst()->AddTask(info);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	TaskInfo info = {};

	info.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	info.Param_0 = (DWORD_PTR)_NextState;

	TaskMgr::GetInst()->AddTask(info);
}

// GameObject.h에 전방선언
bool IsValid(Ptr<GameObject>& _Object)
{
    // 오브젝트가 nullptr이거나, IsDead 상태라면
    // 해당 오브젝트는 제거 되어야합니다.
    if (_Object == nullptr || _Object->IsDead())
    {
        _Object = nullptr;
        return false;
    }

    return true;
}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::RECT;
    info.Pos = _Pos;
    info.Scale = _Scale;
    info.Rotation = _Rot;
    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::RECT;

    // 매개변수로 들어온 world 행렬만 대입
    info.matWorld = _matWorld;
    
    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugSquare(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::SQUARE;

    // 매개변수로 들어온 world 행렬만 대입
    info.matWorld = _matWorld;

    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Duration, bool _DepthTest)
{
    DbgInfo info = {};

    info.Shape = DBG_SHAPE::CIRCLE;
    info.Pos = _Pos;
    info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 0.f);
    info.Rotation = Vec3(0.f, 0.f, 0.f);
    info.Color = _Color;
    info.Age = 0.f;
    info.Life = _Duration;

    info.DepthTest = _DepthTest;

    RenderMgr::GetInst()->AddDebugInfo(info);
}

float Saturate(float _Data)
{
    if (1.f < _Data)
        return 1.f;
    else if (_Data < 0.f)
        return 0.f;
    else
        return _Data;
}

void SwitchingType(PROJ_TYPE& _Type)
{
    _Type = _Type == PROJ_TYPE::ORTHOGRAPHIC ? PROJ_TYPE::PERSPECTIVE : PROJ_TYPE::ORTHOGRAPHIC;
}

void SaveAssetRef(FILE* _File, Asset* _Asset)
{
    /*************************************************************************
    * Asset이 null인 상태(Asset 파생 클래스를 가리키고 있지 않음)로 저장될 수 있기에
    * null인지 아닌지(Asset 파생 클래스를 가리키고 있음) 확인합니다.
    *************************************************************************/
    bool IsNull = _Asset;
    fwrite(&IsNull, sizeof(bool), 1, _File);

    // Asset의 Key, RelativePath 저장
    if (_Asset != nullptr)
    {
        SaveWString(_File, _Asset->GetKey());
        SaveWString(_File, _Asset->GetRelativePath());
    }
}

void SaveWString(FILE* _File, const wstring& _String)
{
    // 문자열 길이만큼 읽어와 파일에 저장
    int Len = _String.length();
    fwrite(&Len, sizeof(int), 1, _File);
    fwrite(_String.data(), sizeof(wchar_t), Len, _File);
}

wstring LoadWString(FILE* _File)
{
    // 파일에 저장된 문자열을 불러와 반환
    int Len = 0;
    fread(&Len, sizeof(int), 1, _File);

    wchar_t buff[255] = {};
    fread(buff, sizeof(wchar_t), Len, _File);

    return buff;
}


#include "GameObject.h"
#include "Device.h"

#include "LevelMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"
#include "CollisionMgr.h"

#include "Source/Scripts/CCamMoveScript.h"
#include "Source/Scripts/CPlayerScript.h"
#include "Source/Scripts/CEnemyScript.h"
#include "Source/Scripts/CEnemySpawner.h"
#include "Source/Scripts/CPlayerFBRController.h"
#include "CSpriteRender.h"
void CreateTestLevel()
{
	/*******************************************
	* Level에 최초 생성할 오브젝트 정보를 정의합니다.
	*******************************************/

	//===========
	// Level 생성
	//===========
	Ptr<ALevel> pLevel = new ALevel;
	pLevel = new ALevel;
	pLevel->SetName(L"Current Level");


	//===============
	// 레이어 이름 설정
	//===============
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(5)->SetName(L"Enermy");
	pLevel->GetLayer(6)->SetName(L"EnermyProjectile");

	pLevel->GetLayer(7)->SetName(L"Plant");
	pLevel->GetLayer(8)->SetName(L"AnchorObject");


	//============
	// 오브젝트 생성
	//============
	Ptr<GameObject> pObject = nullptr;

	#pragma region 카메라, 광원 오브젝트
	//======
	// 카메라
	//======
	pObject = new GameObject;
	pObject->SetName(L"MainCamera");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCamMoveScript);

	pObject->Camera()->LayerCheckAll();
	// Layer 번호 31은 UI 레이어로 설정, editer 로 설정했기 때문에(개발용)
	//pObject->Camera()->LayerCheck(31);

	pObject->Transform()->SetRelativePos({ 0.f, 0.f, -100.f });
	pObject->Transform()->SetRelativeRot({ 0.f, 0.f, 0.f });

	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pObject->Camera()->SetFar(1000.f);
	pObject->Camera()->SetFOV(90.f);
	pObject->Camera()->SetOrthoScale(1.f);
	Vec2 vResolution = Device::GetInst()->GetRenderResolution();
	pObject->Camera()->SetAspectRatio(vResolution.x / vResolution.y);	// 종횡비(AspectRatio)
	pObject->Camera()->SetWidth(vResolution.x);							// height 멤버가 없는대신, width 정보만 설정하고 종횡비를 사용

	// 생성된 오브젝트 등록 (0번 레이어에, 위에 생성한 오브젝트 등록)
	pLevel->AddObject(0, pObject);


	//============
	// 광원 오브젝트
	//============
	pObject = new GameObject;
	pObject->SetName(L"Light");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	pObject->Light2D()->SetRadius(300.f);
	pObject->Light2D()->SetAngle(XM_PI / 7.f);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLevel->AddObject(0, pObject);


	//pObject = new GameObject;
	//pObject->SetName(L"Directional_Light_0");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLight2D);

	//pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pObject->Light2D()->SetLightColor(Vec3(0.5f, 0.5f, 0.5f));
	//pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	//pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//pLevel->AddObject(0, pObject);


	//pObject = new GameObject;
	//pObject->SetName(L"Point_Light_0");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLight2D);

	//pObject->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pObject->Light2D()->SetRadius(300.f);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//pLevel->AddObject(0, pObject);


	//pObject = new GameObject;
	//pObject->SetName(L"Spot_Light_0");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLight2D);

	//pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//// 반지름 설정(범위)
	//pObject->Light2D()->SetRadius(500.f);
	//// 각도는 라디안으로 변환하여 사용
	//// 라디안 변환: PI / 각도
	//pObject->Light2D()->SetAngle(XM_PI / 7.f);

	//pObject->Transform()->SetRelativePos(Vec3(100.f, 0.f, 0.f));

	//pLevel->AddObject(0, pObject);
	#pragma endregion


	//=====
	// 배경
	//=====
	//pObject = new GameObject;
	//pObject->SetName(L"BackGround");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pObject->Transform()->SetRelativeScale(Vec3(800.f, 450.f, 1.f));

	//pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"FullRectMesh"));
	//pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"BGMtrl"));

	//pLevel->AddObject(1, pObject);


	//==================
	// 플레이어 (우주 테마)
	//==================
	//pObject = new GameObject;
	//pObject->SetName(L"Player");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CFlipbookRender);
	//pObject->AddComponent(new CPlayerScript);
	//pObject->AddComponent(new CPlayerFBRController);
	//pObject->AddComponent(new CCollider2D);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, 10.f, -20.f));
	//// 스케일 특정 좌표 * -1을하면 해당 방향이 반전되어 나온다.
	//pObject->Transform()->SetRelativeScale(Vec3(-150.f, 150.f, 1.f));
	//// 회전은 라디안을 사용해야 합니다.
	////pObject->Transform()->SetRelativeRot(Vec3(0.f, 180.f * (XM_PI/180), 0.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotMove.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotIdle.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotAttack.flip"));

	//pObject->FlipbookRender()->Play(1, 8.f, -1);


	//======
	// Enemy
	//======
	//pObject = new GameObject;
	//pObject->SetName(L"Enemy");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CFlipbookRender);
	//pObject->AddComponent(new CEnemyScript);
	//pObject->AddComponent(new CCollider2D);

	//pObject->Transform()->SetRelativePos(Vec3(75.f, 10.f, -20.f));
	//pObject->Transform()->SetRelativeScale(Vec3(25.f, 25.f, 1.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	//// Enemy는 플레이어와 동일한 이미지의 Sprite와 FlipBook을 사용하되, 색상을 다르게 설정합니다.
	////	=> monster.fx에서 color 리턴값을 다르게 설정
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotMove.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotIdle.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\BotAttack.flip"));

	//pObject->FlipbookRender()->Play(1, 8.f, -1);

	//pLevel->AddObject(5, pObject);


	//========
	// TileMap
	//========
	//pObject = new GameObject;
	//pObject->SetName(L"Tile");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CTileRender);


	//pObject->Transform()->SetRelativePos(Vec3(10.f, 10.f, 0.f));
	//pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	//pObject->TileRender()->SetTileMap(LOAD(ATileMap, L"TileMap\\ATileMap.tile"));

	//pLevel->AddObject(1, pObject);


	//========
	// Ground
	//========
	//pObject = new GameObject;
	//pObject->SetName(L"Ground");

	//pObject->AddComponent(new CTransform);
	////pObject->AddComponent(new CSpriteRender);
	//pObject->AddComponent(new CTileRender);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, -410.f, 20.f));
	//// Z축 Scale을 1로 설정해주어야 한다. (자식 오브젝트의 SetIndependentScale에도 영향)
	//pObject->Transform()->SetRelativeScale(Vec3(1600.f, 80.f, 1.f));
	////pObject->SpriteRender()->SetSprite(FIND(ASprite, L"TileSprite_7"));
	//pObject->TileRender()->SetTileMap(LOAD(ATileMap, L"TileMap\\ATileMap.tile"));

	//=======================================
	// Collider 판정을 위한 Ground 자식 오브젝트
	//=======================================
	//int ChildCount = 2;
	//for (int i = 0; i < ChildCount; i++)
	//{
	//	Ptr<GameObject> pChild = new GameObject;

	//	// Collider 역할에 맞는 이름 설정
	//	// 2번만 반복하기에, 3항연산자를 사용 (반복이 추가될 시 다른 조건식으로 변경)
	//	// 0: Platform(발판), 1: Trigger(트리거 이벤트)
	//	wstring name = i == 0 ? L"Platform_Collider" : L"Trigger_Collider";
	//	Vec3 vScale = i == 0 ? Vec3(1600.f, 80.f, 1.f) : Vec3(100.f, 100.f, 1.f);

	//	//wchar_t Buff[50] = {};
	//	//swprintf_s(Buff, L"GroundCollider_%d", i);
	//	pChild->SetName(name);
	//	pChild->AddComponent(new CTransform);
	//	pChild->AddComponent(new CCollider2D);

	//	pChild->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//	pChild->Transform()->SetRelativeScale(vScale);
	//	pChild->Transform()->SetIndependentScale(true);

	//	pObject->AddChild(pChild);
	//}

	//// "Tile" Layer에 등록
	//pLevel->AddObject(2, pObject);


	//========
	// Ground2
	//========
	//pObject = new GameObject;
	//pObject->SetName(L"Ground2");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CSpriteRender);
	//pObject->AddComponent(new CCollider2D);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, -210.f, 20.f));
	//// Z축 Scale을 1로 설정해주어야 한다. (자식 오브젝트의 SetIndependentScale에도 영향)
	//pObject->Transform()->SetRelativeScale(Vec3(800.f, 80.f, 1.f));
	//pObject->SpriteRender()->SetSprite(LOAD(ASprite, L"Sprite\\TileSprite_8.sprite"));

	//pLevel->AddObject(2, pObject);


	//==================
	// MeshRender Object
	//==================
	pObject = new GameObject;
	pObject->SetName(L"MeshRender_Object");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 5.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	pObject->MeshRender()->SetMesh(FIND(AMesh, L"SquareMesh"));
	pObject->MeshRender()->SetMaterial(FIND(AMaterial, L"Std2DMtrl"));

	pLevel->AddObject(1, pObject);


	//=============
	// 레벨 충돌 설정
	//=============
	pLevel->CheckCollisionLayer(4, 5);	// Player 무기 <-> Enemy
	pLevel->CheckCollisionLayer(4, 8);	// Player 무기 <-> Player 자식 오브젝트
	pLevel->CheckCollisionLayer(2, 3);	// Tile <-> Player


	//===============
	// 레벨 변경점 체크
	//===============
	pLevel->SetChanged();

	// 생성한 Level을 Asset으로 등록
	AssetMgr::GetInst()->AddAsset(L"Test_Level", pLevel.Get());

	// TaskMgr에게 다음 프레임에 실행할 Level을 변경하도록 요청
	ChangeLevel(L"Test_Level");
}
