#include "pch.h"
#include "RandomMgr.h"

RandomMgr::RandomMgr()
    : m_vecKeyNum{}
{

}

RandomMgr::~RandomMgr()
{

}

void RandomMgr::Init()
{
    // 벡터에 난수 범위를 미리 담기
    for (UINT i = 0; i < (int)KEY::Z; ++i)
        m_vecKeyNum.push_back(i);
}

KEY RandomMgr::GetRandomKey()
{
    // 난수관련 객체 생성 
    random_device rd;
    mt19937 gen(rd());

    // 벡터를 무작위로 섞기
    shuffle(m_vecKeyNum.begin(), m_vecKeyNum.end(), gen);

    // 앞에서부터 필요한 만큼 가져가기 (중복 절대 없음)
    for (int i = 0; i < 10; ++i) 
    {
        
    }


	KEY key = KEY::KEY_END;

	//random_device rd;
	//mt19937 gen(rd());
	// 무작위는 0 ~ KEY_END 처럼, 범위를 지정 
	// 이동 키를 범위로 지정
	uniform_int_distribution<int> dis((int)KEY::A, (int)KEY::Z);

	int a = dis(gen);

	return KEY();
}
