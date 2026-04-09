#include "pch.h"
#include "SoundMgr.h"

#include "AssetMgr.h"
#include "LevelMgr.h"

SoundMgr::SoundMgr()
{

}

SoundMgr::~SoundMgr()
{

}

void SoundMgr::Init()
{
	// Init 단계에서 미리 사운드 에셋을 로드하여
	// SoundMgr의 자료구조 멤버에 등록합니다.
	m_SoundMap.insert(make_pair(SOUND_TYPE::BGM, FIND(ASound, L"Sound\\BGM\\level_1_8-bit_.wav")));
}

void SoundMgr::Progress()
{
}

void SoundMgr::PlayBGM()
{
	// 사운드 재생 테스트
	m_SoundMap.begin()->second->Play(0, 0.5f, false);

	//pSound->Play(0, 0.5f, false);
}
