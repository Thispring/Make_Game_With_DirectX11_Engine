#include "pch.h"
#include "RandomMgr.h"

int RandomMgr::m_GetterCount = 0;

RandomMgr::RandomMgr()
    : m_vecKeyNum{}
{

}

RandomMgr::~RandomMgr()
{

}

void RandomMgr::Init()
{
    // 중복 초기화 방지 — 재호출 시 기존 데이터 제거
    m_vecKeyNum.clear();
    m_GetterCount = 0;

    // 벡터에 난수 범위를 미리 담기
    for (UINT i = (int)KEY::A; i <= (int)KEY::Z; ++i)
        m_vecKeyNum.push_back(i);
}

void RandomMgr::ShuffleKeyNum()
{
    // 난수관련 객체 생성 
    random_device rd;
    mt19937 gen(rd());

    // 벡터를 무작위로 섞기
    shuffle(m_vecKeyNum.begin(), m_vecKeyNum.end(), gen);

    // 셔플 시점에 카운터를 항상 0으로 초기화하여 drift 방지
    m_GetterCount = 0;
}

KEY RandomMgr::GetRandomKey(int _LoopCount)
{
    assert(m_GetterCount < (int)m_vecKeyNum.size() && "GetRandomKey: 인덱스 범위 초과");

    // 지역에 미리 m_GetterCount 인덱스 값을 집어넣고
    KEY key = (KEY)m_vecKeyNum[m_GetterCount];

    // 횟수를 증가
    ++m_GetterCount;

    // 무작위로 섞인 벡터에서 필요한 만큼 이 함수를 호출합니다.
    // 반복이 필요한 경우 외부 호출자에서 반복 설정
    return key;
}
