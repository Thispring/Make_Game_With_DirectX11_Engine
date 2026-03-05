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