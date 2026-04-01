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

// 다음 프레임에 오브젝트의 활성화 상태를 변경하도록 예약
void SetActiveDeferred(GameObject* _Object, bool _IsActive)
{
	TaskInfo info = {};
	info.Type = TASK_TYPE::SET_ACTIVE_OBJECT;
	info.Param_0 = (DWORD_PTR)_Object;
	info.Param_1 = (DWORD_PTR)_IsActive;

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

void DrawDebugSector(Vec3 _WorldTip, Vec3 _WorldDir, float _WorldRadius, Vec4 _Color, float _Duration, bool _DepthTest)
{
    // ConeMesh_LineStrip은 +Y 방향, 반지름 0.5로 생성됨
    // 1) Scale x2 → 반지름을 _WorldRadius로 맞춤
    // 2) Z 회전  → +Y 축을 _WorldDir로 정렬
    // 3) Translate → 꼭짓점을 _WorldTip으로 이동
    float fAngle = atan2f(-_WorldDir.x, _WorldDir.y);   // +Y → _WorldDir 회전각

    Matrix matScale = XMMatrixScaling(_WorldRadius * 2.f, _WorldRadius * 2.f, 1.f);
    Matrix matRot   = XMMatrixRotationZ(fAngle);
    Matrix matTrans = XMMatrixTranslation(_WorldTip.x, _WorldTip.y, _WorldTip.z);

    DbgInfo info    = {};
    info.Shape      = DBG_SHAPE::SECTOR;
    info.matWorld   = matScale * matRot * matTrans;
    info.Color      = _Color;
    info.Age        = 0.f;
    info.Life       = _Duration;
	info.DepthTest  = _DepthTest;

	RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugLargeBaseCone(Vec3 _WorldTip, Vec3 _WorldDir, float _WorldRadius, Vec4 _Color, float _Duration, bool _DepthTest)
{
	float fAngle = atan2f(-_WorldDir.x, _WorldDir.y);
	Matrix matScale = XMMatrixScaling(_WorldRadius * 2.f, _WorldRadius * 2.f, 1.f);
	Matrix matRot   = XMMatrixRotationZ(fAngle);
	Matrix matTrans = XMMatrixTranslation(_WorldTip.x, _WorldTip.y, _WorldTip.z);

	DbgInfo info    = {};
	info.Shape      = DBG_SHAPE::LARGE_BASE_CONE;
	info.matWorld   = matScale * matRot * matTrans;
	info.Color      = _Color;
	info.Age        = 0.f;
	info.Life       = _Duration;
	info.DepthTest  = _DepthTest;

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

RENDER_DOMAIN StringToDomain(string _DomainName)
{
	if (_DomainName == "DOMAIN_OPAQUE")
		return RENDER_DOMAIN::DOMAIN_OPAQUE;

	if (_DomainName == "DOMAIN_MASKED")
		return RENDER_DOMAIN::DOMAIN_MASKED;

	if (_DomainName == "DOMAIN_TRANSPARENT")
		return RENDER_DOMAIN::DOMAIN_TRANSPARENT;

	if (_DomainName == "DOMAIN_POSTPROCESS")
		return RENDER_DOMAIN::DOMAIN_POSTPROCESS;

	if (_DomainName == "DOMAIN_DEBUG")
		return RENDER_DOMAIN::DOMAIN_DEBUG;

	return RENDER_DOMAIN();
}

COMPONENT_TYPE StringToComponent(string _ComponentName)
{
	if (_ComponentName == "TRANSFORM")
		return COMPONENT_TYPE::TRANSFORM;

	if (_ComponentName == "CAMERA")
		return COMPONENT_TYPE::CAMERA;

	if (_ComponentName == "COLLIDER2D")
		return COMPONENT_TYPE::COLLIDER2D;

	if (_ComponentName == "COLLIDER3D")
		return COMPONENT_TYPE::COLLIDER3D;

	if (_ComponentName == "LIGHT2D")
		return COMPONENT_TYPE::LIGHT2D;

	if (_ComponentName == "LIGHT3D")
		return COMPONENT_TYPE::LIGHT3D;

	if (_ComponentName == "MESHRENDER")
		return COMPONENT_TYPE::MESHRENDER;

	if (_ComponentName == "SPRITE_RENDER")
		return COMPONENT_TYPE::SPRITE_RENDER;

	if (_ComponentName == "BILLBOARD_RENDER")
		return COMPONENT_TYPE::BILLBOARD_RENDER;

	if (_ComponentName == "FLIPBOOK_RENDER")
		return COMPONENT_TYPE::FLIPBOOK_RENDER;

	if (_ComponentName == "PARTICLE_RENDER")
		return COMPONENT_TYPE::PARTICLE_RENDER;

	if (_ComponentName == "TILE_RENDER")
		return COMPONENT_TYPE::TILE_RENDER;

	if (_ComponentName == "END")
		return COMPONENT_TYPE::END;

	if (_ComponentName == "SCRIPT")
		return COMPONENT_TYPE::SCRIPT;

	// 일치하는 문자열이 없을 경우 기본값 반환 (필요에 따라 수정하세요)
	return COMPONENT_TYPE();
}

Component* CreateComponent(COMPONENT_TYPE _Type)
{
	switch (_Type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		return new CTransform;

	case COMPONENT_TYPE::CAMERA:
		return new CCamera;

	case COMPONENT_TYPE::COLLIDER2D:
		return new CCollider2D;

	//case COMPONENT_TYPE::COLLIDER3D:
		//return new CCollider3D;

	case COMPONENT_TYPE::LIGHT2D:
		return new CLight2D;

	//case COMPONENT_TYPE::LIGHT3D:
		//return new CLight3d;

	case COMPONENT_TYPE::MESHRENDER:
		return new CMeshRender;

	case COMPONENT_TYPE::SPRITE_RENDER:
		return new CSpriteRender; // 또는 CSpriterender (클래스명 확인 필요)

	case COMPONENT_TYPE::BILLBOARD_RENDER:
		return new CBillboardRender;

	case COMPONENT_TYPE::FLIPBOOK_RENDER:
		return new CFlipbookRender;

	//case COMPONENT_TYPE::PARTICLE_RENDER:
		//return new CParticle_render;

	case COMPONENT_TYPE::TILE_RENDER:
		return new CTileRender;

	case COMPONENT_TYPE::SCRIPT:
		// 스크립트는 따로 리스트를 만들어서 추가, 관리
		return nullptr;

	case COMPONENT_TYPE::END:
	default:
		return nullptr;
	}
}

wstring ExtractFileName(const wstring& fullPath)
{
	// 1. 마지막 슬래시('\\')의 위치를 찾습니다.
	// 경로가 없는 경우를 대비해 npos일 경우 -1로 취급하여 0번 인덱스부터 시작하게 합니다.
	size_t lastSlashPos = fullPath.find_last_of(L"\\");
	size_t startPos = (lastSlashPos == std::wstring::npos) ? 0 : lastSlashPos + 1;

	// 2. 마지막 언더스코어('_')의 위치를 찾습니다. (숫자나 %d 앞부분)
	size_t lastUnderscorePos = fullPath.find_last_of(L"_");

	// 예외 처리: 언더스코어가 없거나, 언더스코어가 슬래시보다 앞에 있는 경우 (파일명에 _가 없는 경우)
	if (lastUnderscorePos == std::wstring::npos || lastUnderscorePos < startPos) 
	{
		// 언더스코어가 없다면 확장자 점('.') 앞까지만 추출
		size_t lastDotPos = fullPath.find_last_of(L".");
		if (lastDotPos != std::wstring::npos && lastDotPos > startPos) {
			return fullPath.substr(startPos, lastDotPos - startPos);
		}
		// 확장자도 없다면 전체 파일명 반환
		return fullPath.substr(startPos);
	}

	// 3. 시작 위치부터 마지막 언더스코어 직전까지의 길이를 계산하여 추출합니다.
	// 길이 = 끝 위치 - 시작 위치
	return fullPath.substr(startPos, lastUnderscorePos - startPos);
}
