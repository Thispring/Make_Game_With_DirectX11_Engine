#include "pch.h"
#include "PathMgr.h"

vector<wstring> g_vecName;

int main()
{
	CPathMgr::init();
	wstring strProjPath = CPathMgr::GetProjectPath();
	wstring strCppPath = strProjPath + L"GameClient\\Source\\ScriptMgr.cpp";
	wstring strHeaderPath = strProjPath + L"GameClient\\Source\\ScriptMgr.h";

	// 1. 현재 존재하는 모든 스크립트를 알아내야함.
	wstring strScriptIncludePath = CPathMgr::GetProjectPath();
	wstring strScriptCode = strScriptIncludePath + L"GameClient\\Source\\Scripts\\";

	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == handle)
		return 0;

	// 예외 리스트 목록을 알아낸다.
	FILE* pExeptList = nullptr;
	_wfopen_s(&pExeptList, L"exeptlist.txt", L"r");

	vector<wstring> strExept;

	if (nullptr != pExeptList)
	{
		wchar_t szName[255] = L"";

		while (true)
		{
			int iLen = fwscanf_s(pExeptList, L"%s", szName, 255);
			if (iLen == -1)
				break;

			strExept.push_back(szName);
		}
		fclose(pExeptList);
	}

	// 읽기: 추상 클래스 목록 (생성자 호출을 생략할 클래스들)
	FILE* pAbstractList = nullptr;
	_wfopen_s(&pAbstractList, L"Abstract_Class_List.txt", L"r");

	vector<wstring> strAbstract;

	if (nullptr != pAbstractList)
	{
		wchar_t szAbsName[255] = L"";
		while (true)
		{
			int iLen = fwscanf_s(pAbstractList, L"%s", szAbsName, 255);
			if (iLen == -1)
				break;
			strAbstract.push_back(szAbsName);
		}
		fclose(pAbstractList);
	}

	while (true)
	{
		// 예외가 아닌경우, 스크립트 이름으로 본다.
		bool bExeption = false;
		for (size_t i = 0; i < strExept.size(); ++i)
		{
			if (!wcscmp(tData.cFileName, strExept[i].c_str()))
			{
				bExeption = true;
				break;
			}
		}

		if (!bExeption)
		{
			g_vecName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
		}

		if (!FindNextFile(handle, &tData))
			break;
	}

	FindClose(handle);

	FILE* pFile = NULL;

	// =================
	// ScriptMgr h 작성
	// =================
	_wfopen_s(&pFile, strHeaderPath.c_str(), L"w");
	fwprintf_s(pFile, L"#pragma once\n\n");
	fwprintf_s(pFile, L"#include <vector>\n");
	fwprintf_s(pFile, L"#include <string>\n\n");

	fwprintf_s(pFile, L"enum SCRIPT_TYPE\n{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, L"\t");
		fwprintf_s(pFile, strScriptUpperName.c_str());
		fwprintf_s(pFile, L",\n");
	}
	// enum에 END 추가
	fwprintf_s(pFile, L"\n");
	fwprintf_s(pFile, L"\t");
	fwprintf_s(pFile, L"SCRIPT_TYPE_END\n");
	fwprintf_s(pFile, L"};\n\n");

	fwprintf_s(pFile, L"using namespace std;\n\n");
	fwprintf_s(pFile, L"class CScript;\n\n");

	fwprintf_s(pFile, L"class ScriptMgr\n{\n");
	fwprintf_s(pFile, L"public:\n\tstatic void GetScriptInfo(vector<wstring>& _vec);\n");
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(const wstring& _strScriptName);\n");
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(UINT _iScriptType);\n");
	fwprintf_s(pFile, L"\tstatic const wchar_t * GetScriptName(CScript * _pScript);\n");

	// TYPE enum 값을받아, 문자열을 반환하는 함수 추가
	fwprintf_s(pFile, L"\tstatic const char* GetScriptName(SCRIPT_TYPE _Type);\n");

	// const char*를 매개변수로 받아 객체 생성/반환하는 오버로드 선언 추가
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(const char* _strScriptName);\n");

	// 새로 추가: 문자열 -> SCRIPT_TYPE 반환 함수들 (narrow + wide)
	fwprintf_s(pFile, L"\tstatic SCRIPT_TYPE GetScriptType(const char* _strScriptName);\n");
	fwprintf_s(pFile, L"\tstatic SCRIPT_TYPE GetScriptType(const wstring& _strScriptName);\n");

	// 새로 추가: PathMgr 기반으로 .h/.cpp 파일을 생성하는 함수 선언
	fwprintf_s(pFile, L"\tstatic void GenerateScriptFiles(const std::wstring& _strScriptName);\n");

	fwprintf_s(pFile, L"\n};\n");


	fclose(pFile);

	//====================
	// ScriptMgr cpp 작성
	//====================
	_wfopen_s(&pFile, strCppPath.c_str(), L"w");

	// 헤더 입력
	fwprintf_s(pFile, L"#include \"pch.h\"\n");
	fwprintf_s(pFile, L"#include \"ScriptMgr.h\"\n");
	// 문자열 비교 위해 strcmp 사용
	fwprintf_s(pFile, L"#include <cstring>\n\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"#include \"");
		fwprintf_s(pFile, L"Scripts/");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L".h\"\n");
	}

	// 첫 번째 함수 작성
	fwprintf_s(pFile, L"\nvoid ScriptMgr::GetScriptInfo(vector<wstring>& _vec)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\t_vec.push_back(L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\");\n");
	}
	fwprintf_s(pFile, L"}\n\n");


	// 두번째 함수 작성
	fwprintf_s(pFile, L"CScript * ScriptMgr::GetScript(const wstring& _strScriptName)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
        fwprintf_s(pFile, L"\tif (L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\" == _strScriptName)\n");

		// 추상 클래스인 경우에는 동적할당을 생략 (헤더/enum에는 포함됨)
		bool bIsAbstract = false;
		for (size_t ai = 0; ai < strAbstract.size(); ++ai)
		{
			if (g_vecName[i] == strAbstract[ai])
			{
				bIsAbstract = true;
				break;
			}
		}

		if (!bIsAbstract)
		{
			fwprintf_s(pFile, L"\t\treturn NEW ");
			fwprintf_s(pFile, g_vecName[i].c_str());
			fwprintf_s(pFile, L";\n");
		}
		else
		{
			// abstract class: skip instantiation
			fwprintf_s(pFile, L"\t\t// abstract class - instantiation omitted\n");
		}
	}
	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");


	// 세번째 함수
	fwprintf_s(pFile, L"CScript * ScriptMgr::GetScript(UINT _iScriptType)\n{\n");

	fwprintf_s(pFile, L"\tswitch (_iScriptType)\n\t{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, L"\tcase (UINT)SCRIPT_TYPE::");
		fwprintf_s(pFile, strScriptUpperName.c_str());
		fwprintf_s(pFile, L":\n");

		// 추상 클래스이면 인스턴스 반환을 생략하고 주석만 추가
		bool bIsAbstractCase = false;
		for (size_t ai = 0; ai < strAbstract.size(); ++ai)
		{
			if (g_vecName[i] == strAbstract[ai])
			{
				bIsAbstractCase = true;
				break;
			}
		}

		if (!bIsAbstractCase)
		{
			fwprintf_s(pFile, L"\t\treturn NEW ");
			fwprintf_s(pFile, g_vecName[i].c_str());
			fwprintf_s(pFile, L";\n");
		}
		else
		{
			fwprintf_s(pFile, L"\t\t// abstract class - instantiation omitted\n");
		}

		fwprintf_s(pFile, L"\t\tbreak;\n");
	}

	fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}\n\n");

	// 네번째 함수
	fwprintf_s(pFile, L"const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)\n{\n");
	fwprintf_s(pFile, L"\tswitch ((SCRIPT_TYPE)_pScript->GetScriptType())\n\t{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tcase SCRIPT_TYPE::");

		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, strScriptUpperName.c_str());

		fwprintf_s(pFile, L":\n\t\treturn ");
		fwprintf_s(pFile, L"L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\";\n\t\tbreak;\n\n");
	}

	fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}");

	// const char* ScriptMgr::GetScriptName(SCRIPT_TYPE _Type) 함수
	fwprintf_s(pFile, L"\n\nconst char* ScriptMgr::GetScriptName(SCRIPT_TYPE _Type)\n{\n");
	fwprintf_s(pFile, L"\tswitch (_Type)\n\t{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		// enum 이름 생성 (대문자)
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		// 반환할 클래스 이름 (예: "C" + ScriptName)
		wstring classNameW = g_vecName[i];

		// 출력: case CAMMOVESCRIPT: return "CCamMoveScript";
		// (wide 형식을 사용하여 안전하게 삽입)
		fwprintf_s(pFile, L"\tcase %ls: return \"%ls\";\n\t\tbreak;\n\n",
			strScriptUpperName.c_str(), classNameW.c_str());
	}

	// SCRIPT_TYPE_END 케이스 추가
	fwprintf_s(pFile, L"\tcase SCRIPT_TYPE_END: return \"End\";\n\t\tbreak;\n\n");

	// default 및 종료
	fwprintf_s(pFile, L"\tdefault:\n\t\tbreak;\n\t}\n\n\treturn nullptr;\n}\n");


	//====================================================================
	// 추가: const char* 매개변수 받는 GetScript 오버로드 생성 (UTF-8 변환 적용)
	//====================================================================
	fwprintf_s(pFile, L"\n\nCScript * ScriptMgr::GetScript(const char* _strScriptName)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		// g_vecName[i] : std::wstring (UTF-16)
		// 1) wide -> UTF-8 길이 계산 (널 포함)
		int utf8Len = WideCharToMultiByte(CP_UTF8, 0,
			g_vecName[i].c_str(), -1,
			nullptr, 0, nullptr, nullptr);
		std::string scriptNameUtf8;
		if (utf8Len > 0)
		{
			scriptNameUtf8.resize(utf8Len); // 크기에는 널문자까지 포함됨
			WideCharToMultiByte(CP_UTF8, 0,
				g_vecName[i].c_str(), -1,
				&scriptNameUtf8[0], utf8Len, nullptr, nullptr);
			// scriptNameUtf8에는 마지막에 '\0'이 포함되어 있음. fprintf에선 c_str() 사용.
			if (!scriptNameUtf8.empty() && scriptNameUtf8.back() == '\0')
				scriptNameUtf8.pop_back(); // 소스 파일에 쓰일 리터럴에서 중복 널 제거
		}
		else
		{
			// 변환 실패하면 비어있는 ASCII fallback으로 처리
			scriptNameUtf8 = "";
		}

       // 2) 비교문(UTF-8 문자열 리터럴) 출력: narrow 출력 사용
		//    fprintf 사용하여 UTF-8 바이트를 그대로 파일에 쓴다.
		fprintf(pFile, "\tif (0 == strcmp(_strScriptName, \"%s\"))\n", scriptNameUtf8.c_str());

		// 3) abstract 클래스인 경우 인스턴스 생성을 건너뜀
		bool bIsAbstractNarrow = false;
		for (size_t ai = 0; ai < strAbstract.size(); ++ai)
		{
			if (g_vecName[i] == strAbstract[ai])
			{
				bIsAbstractNarrow = true;
				break;
			}
		}

		if (!bIsAbstractNarrow)
		{
			// 3) return NEW ClassName; 은 기존 wide 출력 방식 유지 (클래스명은 ASCII일 가능성이 큼)
			//    클래스 이름은 g_vecName[i] (예: "CCamMoveScript")이므로 wide로 안전하게 출력
			fwprintf_s(pFile, L"\t\treturn NEW %ls;\n", g_vecName[i].c_str());
		}
		else
		{
			fwprintf_s(pFile, L"\t\t// abstract class - instantiation omitted\n");
		}
	}

	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");

	//====================================================================
	// 추가: 문자열 -> SCRIPT_TYPE 반환 함수들 생성 (wstring + const char*)
	//====================================================================

	// 1) wstring 버전: 직접 wide 비교
	fwprintf_s(pFile, L"\nSCRIPT_TYPE ScriptMgr::GetScriptType(const std::wstring& _strScriptName)\n{\n");
	
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		// enum 이름 생성 (대문자)
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		// if (L"ScriptName" == _strScriptName) return ENUM;
		fwprintf_s(pFile, L"\tif (L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\" == _strScriptName)\n");
		fwprintf_s(pFile, L"\t\treturn %ls;\n", strScriptUpperName.c_str());
	}
	fwprintf_s(pFile, L"\treturn SCRIPT_TYPE_END;\n}\n\n");

	// 2) const char* 버전: UTF-8 narrow 비교
	fwprintf_s(pFile, L"\nSCRIPT_TYPE ScriptMgr::GetScriptType(const char* _strScriptName)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		// convert name to UTF-8 narrow literal for strcmp
		int utf8Len = WideCharToMultiByte(CP_UTF8, 0,
			g_vecName[i].c_str(), -1,
			nullptr, 0, nullptr, nullptr);
		std::string scriptNameUtf8;
		if (utf8Len > 0)
		{
			scriptNameUtf8.resize(utf8Len);
			WideCharToMultiByte(CP_UTF8, 0,
				g_vecName[i].c_str(), -1,
				&scriptNameUtf8[0], utf8Len, nullptr, nullptr);
			if (!scriptNameUtf8.empty() && scriptNameUtf8.back() == '\0')
				scriptNameUtf8.pop_back();
		}
		else
		{
			scriptNameUtf8 = "";
		}

		// enum name as narrow ASCII (enum identifiers are ASCII upper letters)
		std::string enumNameA;
		{
			// generate same uppercase identifier as earlier
			wstring tmp = L"";
			for (UINT j = 1; j < g_vecName[i].size(); ++j)
				tmp += toupper(g_vecName[i][j]);
			enumNameA.reserve(tmp.size());
			for (wchar_t wc : tmp) enumNameA.push_back(static_cast<char>(wc));
		}

		// print comparison and return enum
		fprintf(pFile, "\tif (0 == strcmp(_strScriptName, \"%s\"))\n", scriptNameUtf8.c_str());
		fprintf(pFile, "\t\treturn %s;\n", enumNameA.c_str());
	}
	fprintf(pFile, "\treturn SCRIPT_TYPE_END;\n}\n\n");


	//====================================================================
	// 추가: GenerateScriptFiles 구현 생성 (ScriptMgr 내 static 멤버)
	// - 이 함수는 PathMgr에서 얻은 ResPath(Game\\Content\\) 아래에
	//   _strScriptName + ".h" / _strScriptName + ".cpp" 빈 파일을 생성합니다.
	//====================================================================
	// strScriptCode 는 main에서 이미 선언된 std::wstring 경로입니다.
	// 이 값을 소스 코드의 문자열 리터럴로 넣으려면 '\' 문자를 이스케이프(두 개로)해야 합니다.
	std::wstring escapedScriptCode;
	escapedScriptCode.reserve(strScriptCode.size() * 2);
	for (wchar_t wc : strScriptCode) {
		if (wc == L'\\') {
			// 소스 코드에 "\" 를 표현하려면 "\\" 로 써야 하므로 두 개를 추가
			escapedScriptCode.append(L"\\\\");
		}
		else {
			escapedScriptCode.push_back(wc);
		}
	}

	// 이제 생성하는 함수 내부에 이 이스케이프된 문자열을 삽입
	fwprintf_s(pFile, L"\nvoid ScriptMgr::GenerateScriptFiles(const std::wstring& _strScriptName)\n{\n");
	fwprintf_s(pFile, L"\tif (_strScriptName.empty()) return;\n");
	// scriptFolder를 리터럴로 삽입 (이스케이프된 값 사용)
	fwprintf_s(pFile, L"\tstd::wstring scriptFolder = L\"%ls\";\n", escapedScriptCode.c_str());
	fwprintf_s(pFile, L"\tif (scriptFolder.empty()) return;\n\n");

	fwprintf_s(pFile, L"\tstd::wstring headerPath = scriptFolder + _strScriptName + L\".h\";\n");
	fwprintf_s(pFile, L"\tstd::wstring cppPath    = scriptFolder + _strScriptName + L\".cpp\";\n\n");

	fwprintf_s(pFile, L"\tFILE* pH = nullptr;\n");
	fwprintf_s(pFile, L"\tif (_wfopen_s(&pH, headerPath.c_str(), L\"w\") == 0 && pH != nullptr)\n");
	fwprintf_s(pFile, L"\t{\n");
	fwprintf_s(pFile, L"\t\tfclose(pH);\n");
	fwprintf_s(pFile, L"\t}\n");

	fwprintf_s(pFile, L"\tFILE* pC = nullptr;\n");
	fwprintf_s(pFile, L"\tif (_wfopen_s(&pC, cppPath.c_str(), L\"w\") == 0 && pC != nullptr)\n");
	fwprintf_s(pFile, L"\t{\n");
	fwprintf_s(pFile, L"\t\tfclose(pC);\n");
	fwprintf_s(pFile, L"\t}\n");

	fwprintf_s(pFile, L"}\n");

	fclose(pFile);

	return 0;
}