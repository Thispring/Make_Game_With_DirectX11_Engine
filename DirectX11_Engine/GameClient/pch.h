#pragma once
// pch에는 자주 사용하는 헤더 파일을 모아서 관리합니다.

//======================
// Windows, C++ 표준 헤더
//======================
#include <windows.h>
#include <WinUser.h>	// 마우스 휠 사용
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>		// 난수 생성
#include <algorithm>
#include <chrono>
#include <memory>		// C++ 스타일
#include <random>
#include <numeric>

// 파일 입출력
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
// filesystem 헤더에는 흔한 단어로 된 함수가 많아, namespace로 구분합니다.
namespace fs = std::filesystem;

//============
// STL 자료구조
//============
#include <vector>
#include <list>
#include <map>
#include <string>


/*********************************************
* C++ 타입 정보기능
* typeinfo 헤더를 이용해
* 해당 type의 이름이나 고유 번호를 알 수 있다.
* EX) const type_info& info = typeid(Entity);
* const char* pStr = info.name();
* DWORD num = info.hash_code();
*********************************************/
#include <typeinfo>


//============================================
// Direct x11 객체들을 가리킬 수 있는 스마트 포인터
//============================================
#include "wrl.h"
using namespace Microsoft::WRL;

/***************************************************************
* 정적 라이브러리와 동적 라이브러리의 차이는?
* 
* Directx11 라이브러리
* 라이브러리는 빌드된 상태에서 보안을 위해 사용자에게는 전방선언만 제공
***************************************************************/
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;


//===========
// SimpleMath
//===========
#include "SimpleMath.h"
typedef SimpleMath::Vector2 Vec2;
typedef SimpleMath::Vector3 Vec3;
typedef SimpleMath::Vector4 Vec4;
typedef SimpleMath::Matrix Matrix;


/*****************************************************************************************************
* DLL(동적 라이브러리)
* new와 delete 키워드는 CRT 라이브러리에 구현되어 있는 기능으로
* 프로그램 실행 시, CRT와 링크하여 구현된 기능을 수행합니다.
* 하지만 CRT의 어떤 버전을 사용할지, 어떻게 사용할지 DLL마다 차이가 있으므로
* DLL을 사용할때, 생성과 삭제는 해당 DLL에 있는 기능을 활용하고
* DLL을 만들때, 생성과 삭제를 해당 DLL에 구현해줍니다.
*****************************************************************************************************/
#pragma comment(lib, "d3d11.lib")	// 라이브러리 include, 링크 단계에서 위에 선언된 헤더파일을 연결하기 위함
#pragma comment(lib, "d3dcompiler")	
#pragma comment(lib, "dxguid")	


//==================
// DirectTex Library
//==================
#include <DirectxTex\\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectxTex\\DirectXTex.lib")
#endif

//===============
// 사용자 정의 헤더
//===============
// < > 는 IDE가 설정한 구성(절대)경로, " "는 상대경로
#include "resource.h"
#include "global.h"
#include "Ptr.h"
