#pragma once

void CreateObject(class GameObject* _Object, int LayerIdx);
void ChangeLevel(const wstring& _NextLevelName);
void ChangeLevelState(LEVEL_STATE _NextState);

//============
// DebugRender
//============
// bool _DepthTest = false <- DebugRender를 그릴지 말지를 bool 디폴트 변수로 분기처리
void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Duration, bool _DepthTest = false);
// World 행렬을 받는 버전
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest = false);
// 정사각형
void DrawDebugSquare(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest = false);
// 원형 Render
void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Duration, bool _DepthTest = false);
// 부채꼴 Render (월드 공간 꼭짓점, 방향, 반지름)
void DrawDebugSector(Vec3 _WorldTip, Vec3 _WorldDir, float _WorldRadius, Vec4 _Color, float _Duration, bool _DepthTest = false);
//void DrawDebugLargeBaseCone(Vec3 _WorldTip, Vec3 _WorldDir, float _WorldRadius, Vec4 _Color, float _Duration, bool _DepthTest = false);
// World 행렬을 직접 받는 오버로드 (충돌 판정과 정확히 일치)
void DrawDebugLargeBaseCone(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DepthTest = false);


// 0 ~ 1로 제한하는 함수
float Saturate(float _Data);

// 다음 프레임에 오브젝트의 활성화 상태를 변경하도록 예약
void SetActiveDeferred(class GameObject* _Object, bool _IsActive);

#include "CCamera.h"
// PROJ_TYPE을 스위칭하는 함수
// 호출했을 때, m_ProjType 여부를 확인하고 다른 투영으로 변경
void SwitchingType(PROJ_TYPE& _Type);


// Asset을 파일로 저장, 불러오는 함수
void SaveAssetRef(FILE* _File, class Asset* _Asset);
// File에, 특정 객체(Shader, Texture...)의 문자열을 저장, 불러오는 함수
void SaveWString(FILE* _File, const wstring& _String);
wstring LoadWString(FILE* _File);

// String 문자열을 받아, 알맞은 enum class RENDER_DOMAIN을 반환하는 함수
RENDER_DOMAIN StringToDomain(string _DomainName);

// String 문자열을 받아, 알맞은 COMPONENT_TYPE을 반환하는 함수
COMPONENT_TYPE StringToComponent(string _ComponentName);

// COMPONENT_TYPE를 받아 조건에 맞는 Component 객체를 생성하는 함수
Component* CreateComponent(COMPONENT_TYPE _Type);

// Asset의 Key값 문자열을 받았을때, 경로와 확장자를 제거한 이름을 반환하는 함수
// Sprite에서 _%d와 앞의 파일 확장자를 제거하기 위해 사용
wstring ExtractFileName(const wstring& fullPath);
