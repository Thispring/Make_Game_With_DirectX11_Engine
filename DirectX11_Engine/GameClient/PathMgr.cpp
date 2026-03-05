#include "pch.h"
#include "PathMgr.h"
#include "Engine.h"

PathMgr::PathMgr()
	: m_ContentPath{}
{
	Init();
}

PathMgr::~PathMgr()
{

}

void PathMgr::Init()
{
	/********************************************************************
	* ContentPath 경로 찾기
	* 실행파일이 있는 Bin 폴더 경로를 찾아냅니다.
	* 디버깅 모드에서도 똑같이 동작하게 하기 위해서, 
	* 프로젝트 구성설정, 디버깅 탭에 작업 디렉터리를 실행파일 경로로 설정해줍니다.
	********************************************************************/
	GetCurrentDirectory(255, m_ContentPath);	// <= 실행파일의 경로를 알 수 있음
	// (26-03-02): Visual Studio의 프로젝트 설정이 아래와 같이 설정되지 않으면, 의도한 경로 출력이 되지 않음
	// 프로젝트 설정 -> 구성 속성 -> 디버깅 -> 작업 디렉터리 -> "$(SolutionDir)Game\Bin\" 
	//GetModuleFileName(NULL, m_ContentPath, 255);	// 현재 실행 파일의 '전체 경로'를 얻어오는 함수, 사용 시 아래 for문 2회 실행 필수

	int Len = wcslen(m_ContentPath);

	for (int i = Len - 1; 0 <= i; --i)
	{
		// Path 경로 문자열을 거꾸로 순회하여,
		// 원하는 폴더 경로로 만드는 반복문
		if ('\\' == m_ContentPath[i])
		{
			m_ContentPath[i] = '\0';
			break;
		}
	}

	// GetModuleFileName 사용 시 Bin 폴더 까지 잡히기 때문에 2번 반복
	//Len = wcslen(m_ContentPath);	// 짧아진 문자열을 다시 계산
	//for (int i = Len - 1; 0 <= i; --i)
	//{
	//	// Path 경로 문자열을 거꾸로 순회하여,
	//	// 원하는 폴더 경로로 만드는 반복문
	//	if ('\\' == m_ContentPath[i])
	//	{
	//		m_ContentPath[i] = '\0';
	//		break;
	//	}
	//}

	wcscat_s(m_ContentPath, L"\\Content\\");
}
