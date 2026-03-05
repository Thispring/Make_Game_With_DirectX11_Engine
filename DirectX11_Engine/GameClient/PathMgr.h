#pragma once

// 프로젝트에 생성한 폴더 경로를 관리합니다.
class PathMgr
	: public Singleton<PathMgr>
{
	SINGLE(PathMgr)
private:
	// Windows에서 경로 글자수를 255로 제한하기 때문에 255 배열로 선언합니다.
	wchar_t m_ContentPath[255];

public:
	//=========
	// 멤버 함수
	//=========
	void Init();


	//=========
	// Get, Set
	//=========
	const wchar_t* GetContentPath() { return m_ContentPath; }
};

// 자주 사용되는 함수 재정의
#define CONTENT_PATH PathMgr::GetInst()->GetContentPath()
