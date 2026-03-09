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
#include "Source/Scripts/CPlayerStatus.h"
#include "Source/Scripts/CPlayerController.h"
#include "Source/Scripts/CPlayerAnimator.h"
#include "Source/Scripts/CEnemySpawner.h"
#include "Source/Scripts/CEnemyStatus.h"
#include "Source/Scripts/CEnemyBehavior.h"
#include "Source/Scripts/CEnemyAnimator.h"
#include "CSpriteRender.h"
void CreateLevel()
{
	//{
	//	// 파일로 저장된 Level 불러오기
	//	Ptr<ALevel> pLevel = LOAD(ALevel, L"Level\\Normal_Stage_0.lv");
	//	ChangeLevel(L"Level\\Normal_Stage_0.lv");
	//}

	//return;


	/*******************************************
	* Level에 최초 생성할 오브젝트 정보를 정의합니다.
	*******************************************/

	//===========
	// Level 생성
	//===========
	Ptr<ALevel> pLevel = nullptr;
	pLevel = new ALevel;
	pLevel->SetName(L"Normal_Stage_0");


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

	pObject->Transform()->SetRelativePos({ 0.f, 0.f, -350.f });
	pObject->Transform()->SetRelativeRot({ 0.f, 0.f, 0.f });

	pObject->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
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
	pObject->SetName(L"DirectLight");
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


	#pragma region BackGround
	// BackGround
	// Render가 없는 빈 오브젝트 BackGround를 만들고, 
	// 자식 오브젝트들에 Render를 추가 하여 빈 오브젝트 자식으로 등록
	
	// BackGround Empty Object
	pObject = new GameObject;
	pObject->SetName(L"BackGround");

	pObject->AddComponent(new CTransform);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	// Sky
	Ptr<GameObject> pChild = nullptr;
	pChild = new GameObject;
	pChild->SetName(L"Sky");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pChild->Transform()->SetRelativeScale(Vec3(800.f, 450.f, 1.f));

	pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"FullRectMesh"));
	pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"SkyMtrl"));

	pObject->AddChild(pChild);

	// Cloud
	pChild = new GameObject;
	pChild->SetName(L"Cloud");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetRelativePos(Vec3(0.f, -100.f, 98.f));
	pChild->Transform()->SetRelativeScale(Vec3(800.f, 400.f, 1.f));

	pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"FullRectMesh"));
	pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"CloudMtrl"));

	pObject->AddChild(pChild);

	// Sea
	pChild = new GameObject;
	pChild->SetName(L"Sea");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetRelativePos(Vec3(0.f, -500.f, 96.f));
	pChild->Transform()->SetRelativeScale(Vec3(800.f, 400.f, 1.f));

	pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"FullRectMesh"));
	pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"SeaMtrl"));

	pObject->AddChild(pChild);

	// Far Grounds
	pChild = new GameObject;
	pChild->SetName(L"Ground");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetRelativePos(Vec3(0.f, -250.f, 94.f));
	pChild->Transform()->SetRelativeScale(Vec3(800.f, 200.f, 1.f));

	pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"FullRectMesh"));
	pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"GroundMtrl"));

	pObject->AddChild(pChild);

	pLevel->AddObject(1, pObject);
	#pragma endregion


	#pragma region Player
	// Player
	pObject = new GameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CPlayerStatus);
	pObject->AddComponent(new CPlayerController);
	pObject->AddComponent(new CPlayerAnimator);
	pObject->AddComponent(new CCollider2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 10.f, -50.f));
	// 스케일 특정 좌표 * -1을하면 해당 방향이 반전되어 나온다.
	pObject->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
	// 회전은 라디안을 사용해야 합니다.
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 180.f * (XM_PI/180), 0.f));
	pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	pObject->Collider2D()->SetScale(Vec2(0.2f, 0.25f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, -0.05f));
	
	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\cat_Idle.flip"));
	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\cat_Walk.flip"));
	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\cat_Jump.flip"));
	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\cat_OTcombo.flip"));

	pObject->FlipbookRender()->Play(0, 8.f, -1);
	pLevel->AddObject(3, pObject);
	#pragma endregion

	//======
	// Enemy
	//======
	pObject = new GameObject;
	pObject->SetName(L"Enemy");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CEnemyStatus);
	pObject->AddComponent(new CEnemyBehavior);
	pObject->AddComponent(new CEnemyAnimator);
	pObject->AddComponent(new CCollider2D);

	pObject->Transform()->SetRelativePos(Vec3(100.f, 10.f, -50.f));
	pObject->Transform()->SetRelativeScale(Vec3(-100.f, 100.f, 1.f));
	pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));


	pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\mon1_Idle.flip"));

	pObject->FlipbookRender()->Play(0, 8.f, -1);

	pLevel->AddObject(5, pObject);


	// MeshRender Object
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

	// Tile 2번 레이어 등록
	pObject = new GameObject;
	pObject->SetName(L"TileGround");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, -80.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(800.f, 10.f, 0.f));
	//pObject->MeshRender()->SetMesh(FIND(AMesh, L"SquareMesh"));
	//pObject->MeshRender()->SetMaterial(FIND(AMaterial, L"Std2DMtrl"));

	pLevel->AddObject(2, pObject);

	//=============
	// 레벨 충돌 설정
	//=============
	pLevel->CheckCollisionLayer(2, 3);	// Tile <-> Player
	pLevel->CheckCollisionLayer(2, 5);	// Tile <-> Enemy


	//===============
	// 레벨 변경점 체크
	//===============
	pLevel->SetChanged();

	// 생성한 Level을 Asset으로 등록
	AssetMgr::GetInst()->AddAsset(L"Normal_Stage_0", pLevel.Get());

	// Level을 파일로 저장
	wstring ContentPath = CONTENT_PATH;
	pLevel->Save(ContentPath + L"Level\\Normal_Stage_0.lv");

	// TaskMgr에게 다음 프레임에 실행할 Level을 변경하도록 요청
	ChangeLevel(L"Normal_Stage_0");
}
