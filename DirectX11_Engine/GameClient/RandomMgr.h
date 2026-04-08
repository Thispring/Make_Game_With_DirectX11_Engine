#pragma once
#include "KeyMgr.h"

//// 연속된 KEY 배열 정리:
//enum class KEY_ARRAY 
//{
//	QWE, WER, ERT, RTY, TYU, YUI, UIO, IOP,
//	ASD, SDF, DFG, FGH, GHJ, HJK, JKL,
//	ZXC, XCV, CVB, VBN, BNM,
//};


//=============================================
// 게임 콘텐츠로 사용될 난수들을 관리합니다.
// ex) Player 부활 시, 공격 KEY 값 랜덤으로 전달
//=============================================
class RandomMgr
	: public Singleton<RandomMgr>
{
	SINGLE(RandomMgr)
private:
	static int		m_GetterCount;	// 몇 번 반환되는지를 기록하는 멤버
	vector<int>		m_vecKeyNum;

public:
	void Init();

	// vector를 셔플
	void ShuffleKeyNum();
	// 무작위 KEY를 반환하는 함수
	KEY GetRandomKey(int _LoopCount);


	void GetRandomKeyArray(KEY& _First, KEY& _Second, KEY& _Third);
};
