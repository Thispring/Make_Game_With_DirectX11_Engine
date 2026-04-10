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
	m_SoundGroups[SOUND_TYPE::BGM][L"level_1_8-bit_"] = FIND(ASound, L"Sound\\BGM\\level_1_8-bit_.wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"Interstellar Fleet 1 [LOOP]"] = FIND(ASound, L"Sound\\BGM\\Interstellar Fleet 1 [LOOP].wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"it_takes_a_hero"] = FIND(ASound, L"Sound\\BGM\\it_takes_a_hero.wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"Haran"] = FIND(ASound, L"Sound\\BGM\\Haran.mp3").Get();


}

void SoundMgr::Progress()
{
}


void SoundMgr::PlayBGM(wstring _LevelName)
{
	// Level 이름에 따라 BGM 분리 재생
	if (_LevelName == L"Level\\MainMenu.lv")
		m_SoundGroups[SOUND_TYPE::BGM][L"Interstellar Fleet 1 [LOOP]"]->Play(0, 0.5f, false);
	else if (_LevelName == L"Level\\Normal_Stage_0.lv")
		m_SoundGroups[SOUND_TYPE::BGM][L"level_1_8-bit_"]->Play(0, 0.5f, false);
	else if (_LevelName == L"Level\\Ending.lv")
		m_SoundGroups[SOUND_TYPE::BGM][L"it_takes_a_hero"]->Play(0, 0.5f, false);
	else if (_LevelName == L"Level\\GameOver.lv")
		m_SoundGroups[SOUND_TYPE::BGM][L"Haran"]->Play(0, 0.5f, false);

}

void SoundMgr::StopPrevBGM()
{
	// BGM 그룹의 모든 사운드를 순회하며 재생 중인 사운드는 정지
	for (auto& pair : m_SoundGroups[SOUND_TYPE::BGM])
	{
		Ptr<ASound> pSound = pair.second;
		if (pSound->IsPlaying()) // IsPlaying()이 있으면 주석 해제
		{
			pSound->Stop();
		}
	}
}

void SoundMgr::PlaySFX(wstring _SFXName)
{
	// wstring Key값을 매개변수로 받아서
	// 해당되는 SFX 재생
}
