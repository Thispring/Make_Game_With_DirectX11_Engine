#pragma once
#include "KeyMgr.h"

//=============================================
// 게임 콘텐츠로 사용될 난수들을 관리합니다.
// ex) Player 부활 시, 공격 KEY 값 랜덤으로 전달
//=============================================
class RandomMgr
	: public Singleton<RandomMgr>
{
	SINGLE(RandomMgr)
private:
	vector<int>		m_vecKeyNum;

public:
	void Init();

	// 무작위 KEY를 반환하는 함수
	KEY GetRandomKey();
};
