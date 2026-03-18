#pragma once
// 자주 사용하는 매크로 부분을 모아서 관리합니다.

// 싱글턴 매크로
#define SINGLE(Type) private:\
						Type();\
						Type(const Type& _Other) = delete;\
						~Type();\
						friend class Singleton<Type>;

// 최대 레이어 보유 개수
#define MAX_LAYER 32

// 자주 선언되는 Get, Set 함수를 매크로 정의
#define GET(Type, MemName) Type Get##MemName(){return m_##MemName;}
#define SET(Type, MemName) void Set##MemName(Type _Data){ m_##MemName = _Data;}
#define GET_SET(Type, MemName) GET(Type, MemName) SET(Type, MemName)

// File 스트림 생성과 열기 
#define FILE_OPEN(Path, Mode) FILE* pFile = nullptr;\
						      _wfopen_s(&pFile, Path.c_str(), Mode);

// 객체 복사 매크로
#define CLONE(Type) Type* Clone() { return new Type(*this); }

// ImGui UI 간격 조정 매크로
#define SPACING_UI(Count) for (int i = 0; i < Count; ++i) ImGui::Spacing();

// RGB값을 입력하면 Vector로 변환하는 매크로
// 입력값 / 255.0f;
#define ColorConvertIntToVec4(r, g, b) Vec4((float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f, 1.0f)

// ImGui Text로 필수인지 선택인지를 표시하는 매크로
#define IMGUI_REQUIRED() ImGui::TextColored(ImVec4(1.f, 0.2f, 0.2f, 1.0f), "(Required)");
#define IMGUI_OPTIONAL() ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "(Optional)");
