#pragma once
#include "Entity.h"

// Asset이란?
// Asset을 상속받는 다른 class 이름 앞에 'A'를 붙여줍니다.
class Asset
	: public Entity
{

private:
	/***********************************************
	* 의도에 맞게 Asset의 경로를 바꿀 수 있도록 상수화 X
	* 단, 함부로 바꾸지 못하도록 알맞은 멤버함수 구현
	***********************************************/
	wstring m_Key;					// 에셋을 찾을 때 사용할 이름
	wstring m_RelativePath;			// 상대 경로

	/*******************************************
	* RTTI: RunTime Type Identification:
	* 런타임 도중에, 객체의 실제 타입을 알아내는 기술
	*******************************************/
	const ASSET_TYPE m_Type;		// 에셋 타입


	//=================
	// private 멤버 함수
	//=================
	// Asset을 상속받는 클래스들은 Key와 Path를 입력받도록 설정
	void SetKey(const wstring& _Key) { m_Key = _Key; }
	void SetRelativePath(const wstring& _Path) { m_RelativePath = _Path; }

public:
	//=========
	// 멤버 함수
	//=========
	CLONE(Asset);


	//=============
	// 상속 멤버 함수
	//=============
	// File I/O 관련 함수 (저장, 불러오기)
	// 해당 기능이 필요한 Asset들만 구현하도록 설계
	virtual int Save(const wstring& _FilePath) { return S_OK; };
	virtual int Load(const wstring& _FilePath) { return S_OK; };


	//=========
	// Get, Set
	//=========
	const wstring& GetKey() { return m_Key; }
	const wstring& GetRelativePath() { return m_RelativePath; }
	ASSET_TYPE GetType() { return m_Type; }

	
	//===========
	// 생성, 소멸자
	//===========
	/*********************************************
	* 자신이 어떤 타입의 Asset인지 입력으로 받는 버전의
	* 생성자만 남겨둠 (기본 생성자 호출 X)
	*********************************************/
	Asset(ASSET_TYPE _Type);
	// Asset 클래스 복사생성자
	Asset(const Asset& _Origin);
	virtual ~Asset();


	//=============
	// friend class
	//=============
	friend class AssetMgr;
};
