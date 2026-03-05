#pragma once
// 자주 사용되는 함수를 전방선언 합니다.
// 기능은 func.cpp에서 구현

//========
// TeskMgr
//========
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

// 0 ~ 1로 제한하는 함수
float Saturate(float _Data);

#include "CCamera.h"
// PROJ_TYPE을 스위칭하는 함수
// 호출했을 때, m_ProjType 여부를 확인하고 다른 투영으로 변경
void SwitchingType(PROJ_TYPE& _Type);


// Asset을 파일로 저장, 불러오는 함수
void SaveAssetRef(FILE* _File, class Asset* _Asset);
// File에, 특정 객체(Shader, Texture...)의 문자열을 저장, 불러오는 함수
void SaveWString(FILE* _File, const wstring& _String);
wstring LoadWString(FILE* _File);

// Engine에 사용 할 기본 Level을 생성하는 함수
void CreateTestLevel();
