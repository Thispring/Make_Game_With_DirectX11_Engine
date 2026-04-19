#include "pch.h"
#include "SoundMgr.h"

#include "AssetMgr.h"
#include "LevelMgr.h"
#include "TimeMgr.h"

SoundMgr::SoundMgr()
	: m_isBGMMute(false)
	, m_isSFXMute(false)
	, m_BGMVolume(1.f)
	, m_SFXVolume(1.f)
	, m_prevBGMVolume(1.f)
	, m_prevSFXVolume(1.f)
	, m_bBGMChangedWhileMuted(false)
	, m_bSFXChangedWhileMuted(false)
{

}

// 전체 BGM 볼륨을 일괄 적용 (0..1)
void SoundMgr::SetBGMVolume(float _f)
{
    // If currently muted, record the change but don't apply to channels
	if (m_isBGMMute)
	{
		m_BGMVolume = _f;
		m_bBGMChangedWhileMuted = true;
		return;
	}

	m_BGMVolume = _f;
	for (auto& pair : m_SoundGroups[SOUND_TYPE::BGM])
	{
		Ptr<ASound> pSound = pair.second;
		if (pSound != nullptr)
			pSound->SetVolumeAll(_f);
	}
}

// 전체 BGM 음소거 토글
void SoundMgr::SetBGMMute(bool _bMute)
{
    if (_bMute)
	{
		// transitioning to mute
		if (!m_isBGMMute)
		{
			m_prevBGMVolume = m_BGMVolume;
			m_bBGMChangedWhileMuted = false;
		}

		m_isBGMMute = true;
		for (auto& pair : m_SoundGroups[SOUND_TYPE::BGM])
		{
			Ptr<ASound> pSound = pair.second;
			if (pSound != nullptr)
				pSound->SetMute(true);
		}
	}
	else
	{
		// unmute: restore volume depending on whether user changed slider while muted
		m_isBGMMute = false;
		if (!m_bBGMChangedWhileMuted)
		{
			m_BGMVolume = m_prevBGMVolume;
		}

		for (auto& pair : m_SoundGroups[SOUND_TYPE::BGM])
		{
			Ptr<ASound> pSound = pair.second;
			if (pSound != nullptr)
			{
				pSound->SetMute(false);
				pSound->SetVolumeAll(m_BGMVolume);
			}
		}

		m_bBGMChangedWhileMuted = false;
	}
}

bool SoundMgr::IsBGMMute() const
{
	return m_isBGMMute;
}

// SFX 제어 (동일한 방식)
void SoundMgr::SetSFXVolume(float _f)
{
    if (m_isSFXMute)
	{
		m_SFXVolume = _f;
		m_bSFXChangedWhileMuted = true;
		return;
	}

	m_SFXVolume = _f;
	for (auto& pair : m_SoundGroups[SOUND_TYPE::SFX])
	{
		Ptr<ASound> pSound = pair.second;
		if (pSound != nullptr)
			pSound->SetVolumeAll(_f);
	}
}

void SoundMgr::SetSFXMute(bool _bMute)
{
    if (_bMute)
	{
		if (!m_isSFXMute)
		{
			m_prevSFXVolume = m_SFXVolume;
			m_bSFXChangedWhileMuted = false;
		}

		m_isSFXMute = true;
		for (auto& pair : m_SoundGroups[SOUND_TYPE::SFX])
		{
			Ptr<ASound> pSound = pair.second;
			if (pSound != nullptr)
				pSound->SetMute(true);
		}
	}
	else
	{
		m_isSFXMute = false;
		if (!m_bSFXChangedWhileMuted)
		{
			m_SFXVolume = m_prevSFXVolume;
		}

		for (auto& pair : m_SoundGroups[SOUND_TYPE::SFX])
		{
			Ptr<ASound> pSound = pair.second;
			if (pSound != nullptr)
			{
				pSound->SetMute(false);
				pSound->SetVolumeAll(m_SFXVolume);
			}
		}

		m_bSFXChangedWhileMuted = false;
	}
}

bool SoundMgr::IsSFXMute() const
{
	return m_isSFXMute;
}

SoundMgr::~SoundMgr()
{

}

void SoundMgr::Init()
{
	// Init 단계에서 미리 사운드 에셋을 로드하여
	// SoundMgr의 자료구조 멤버에 등록합니다.
	m_SoundGroups[SOUND_TYPE::BGM][L"Normal_Stage_0"] = FIND(ASound, L"Sound\\BGM\\level_1_8-bit_.wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"MainMenu"] = FIND(ASound, L"Sound\\BGM\\Interstellar Fleet 1 [LOOP].wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"Ending"] = FIND(ASound, L"Sound\\BGM\\it_takes_a_hero.wav").Get();
	m_SoundGroups[SOUND_TYPE::BGM][L"GameOver"] = FIND(ASound, L"Sound\\BGM\\Haran.wav").Get();

	// Player
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerJump"] = FIND(ASound, L"Sound\\SFX\\PlayerJump.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerMove"] = FIND(ASound, L"Sound\\SFX\\PlayerMove.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerPunch1"] = FIND(ASound, L"Sound\\SFX\\PlayerPunch1.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerPunch2"] = FIND(ASound, L"Sound\\SFX\\PlayerPunch2.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerKick"] = FIND(ASound, L"Sound\\SFX\\PlayerKick.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerDeath"] = FIND(ASound, L"Sound\\SFX\\PlayerDeath.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"PlayerRespawn"] = FIND(ASound, L"Sound\\SFX\\PlayerRespawn.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"EnergyBlast"] = FIND(ASound, L"Sound\\SFX\\EnergyBlast.wav").Get();

	m_SoundGroups[SOUND_TYPE::SFX][L"Level_Move"] = FIND(ASound, L"Sound\\SFX\\Level_Move.wav").Get();
	
	m_SoundGroups[SOUND_TYPE::SFX][L"SavePoint"] = FIND(ASound, L"Sound\\SFX\\SavePoint.wav").Get();
	
	// Enemy
	// DEMON
	m_SoundGroups[SOUND_TYPE::SFX][L"DEMON_Hit"] = FIND(ASound, L"Sound\\SFX\\DEMON_Hit.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"DEMON_Chase"] = FIND(ASound, L"Sound\\SFX\\DEMON_Chase.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"DEMON_Attack"] = FIND(ASound, L"Sound\\SFX\\DEMON_Attack.wav").Get();

	// SKULL
	m_SoundGroups[SOUND_TYPE::SFX][L"SKULL_Hit"] = FIND(ASound, L"Sound\\SFX\\SKULL_Hit.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"SKULL_Chase"] = FIND(ASound, L"Sound\\SFX\\SKULL_Chase.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"SKULL_Attack"] = FIND(ASound, L"Sound\\SFX\\SKULL_Attack.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"SKULL_Ghost_In"] = FIND(ASound, L"Sound\\SFX\\SKULL_Ghost_In.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"SKULL_Ghost_Idle"] = FIND(ASound, L"Sound\\SFX\\SKULL_Ghost_Idle.wav").Get();
	
	// FLYING
	m_SoundGroups[SOUND_TYPE::SFX][L"FLYING_Hit"] = FIND(ASound, L"Sound\\SFX\\FLYING_Hit.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"FLYING_Attack"] = FIND(ASound, L"Sound\\SFX\\FLYING_Attack.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"FLYING_Chase"] = FIND(ASound, L"Sound\\SFX\\FLYING_Chase.wav").Get();
	
	// FLOWER
	m_SoundGroups[SOUND_TYPE::SFX][L"FLOWER_Hit"] = FIND(ASound, L"Sound\\SFX\\FLOWER_Hit.wav").Get();
	m_SoundGroups[SOUND_TYPE::SFX][L"FLOWER_RangedAttack"] = FIND(ASound, L"Sound\\SFX\\FLOWER_RangedAttack.wav").Get();

}

void SoundMgr::Progress()
{
}


void SoundMgr::PlayBGM(wstring _LevelName)
{
	if (m_isBGMMute)
		return;

	// Level 이름에 따라 BGM 분리 재생
    float fPlayVolume = m_isBGMMute ? 0.f : m_BGMVolume;

	if (_LevelName == L"Level\\MainMenu.lv")
	{
    Ptr<ASound> pSound = m_SoundGroups[SOUND_TYPE::BGM][L"MainMenu"];
	if (pSound != nullptr)
		{
			pSound->Play(0, fPlayVolume, false);
			if (m_isBGMMute)
				pSound->SetMute(true);
		}
	}
	else if (_LevelName == L"Level\\Normal_Stage_0.lv")
	{
    Ptr<ASound> pSound = m_SoundGroups[SOUND_TYPE::BGM][L"Normal_Stage_0"];
	if (pSound != nullptr)
		{
			pSound->Play(0, fPlayVolume, false);
			if (m_isBGMMute)
				pSound->SetMute(true);
		}
	}
	else if (_LevelName == L"Level\\Ending.lv")
	{
    Ptr<ASound> pSound = m_SoundGroups[SOUND_TYPE::BGM][L"Ending"];
	if (pSound != nullptr)
		{
			pSound->Play(0, fPlayVolume, false);
			if (m_isBGMMute)
				pSound->SetMute(true);
		}
	}
	else if (_LevelName == L"Level\\GameOver.lv")
	{
    Ptr<ASound> pSound = m_SoundGroups[SOUND_TYPE::BGM][L"GameOver"];
	if (pSound != nullptr)
		{
			pSound->Play(0, fPlayVolume, false);
			if (m_isBGMMute)
				pSound->SetMute(true);
		}
	}

}

void SoundMgr::StopPrevBGM()
{
	// BGM 그룹의 모든 사운드를 순회하며 재생 중인 사운드는 정지
	for (auto& pair : m_SoundGroups[SOUND_TYPE::BGM])
	{
    Ptr<ASound> pSound = pair.second;
	if (pSound != nullptr && pSound->IsPlaying()) // IsPlaying()이 있으면 주석 해제
		{
			pSound->Stop();
		}
	}
}

void SoundMgr::PlaySFX(wstring _SFXName)
{
    // default wrapper -> allows single-shot overlapping
	PlaySFX(_SFXName, 1, true);
}

void SoundMgr::PlaySFX(wstring _SFXName, int _iRoopCount, bool _bOverlap)
{
	if (m_isSFXMute)
		return;

	auto& sfxGroup = m_SoundGroups[SOUND_TYPE::SFX];
	auto iter = sfxGroup.find(_SFXName);
	if (iter == sfxGroup.end())
		return;

	Ptr<ASound> pSound = iter->second;
	if (pSound == nullptr)
		return;

	float fPlayVolume = m_isSFXMute ? 0.f : m_SFXVolume;
	pSound->Play(_iRoopCount, fPlayVolume, _bOverlap);
	if (m_isSFXMute)
		pSound->SetMute(true);
}

void SoundMgr::PlaySFX(wstring _SFXName, float _minIntervalSeconds, int _iRoopCount, bool _bOverlap)
{
	if (m_isSFXMute)
		return;

	// If a minimum interval is specified, enforce cooldown per SFX name
	if (_minIntervalSeconds > 0.f)
	{
		float now = TimeMgr::GetInst()->GetTime();
		auto it = m_SFXLastPlayTime.find(_SFXName);
		if (it != m_SFXLastPlayTime.end())
		{
			if (now - it->second < _minIntervalSeconds)
				return; // too soon, drop request
		}
		m_SFXLastPlayTime[_SFXName] = now;
	}

	auto& sfxGroup = m_SoundGroups[SOUND_TYPE::SFX];
	auto iter = sfxGroup.find(_SFXName);
	if (iter == sfxGroup.end())
		return;

	Ptr<ASound> pSound = iter->second;
	if (pSound == nullptr)
		return;

	float fPlayVolume = m_isSFXMute ? 0.f : m_SFXVolume;
	pSound->Play(_iRoopCount, fPlayVolume, _bOverlap);
	if (m_isSFXMute)
		pSound->SetMute(true);
}

void SoundMgr::StopSFX(wstring _SFXName)
{
	auto& sfxGroup = m_SoundGroups[SOUND_TYPE::SFX];
	auto iter = sfxGroup.find(_SFXName);
	if (iter == sfxGroup.end())
		return;

	Ptr<ASound> pSound = iter->second;
	if (pSound == nullptr)
		return;

	pSound->Stop();
}

	