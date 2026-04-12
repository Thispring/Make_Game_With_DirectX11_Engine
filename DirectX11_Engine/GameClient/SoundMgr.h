#pragma once
#include "ASound.h"

// 게임 사운드를 관리하는 싱글턴 매니저 클래스
class SoundMgr
	: public Singleton<SoundMgr>
{
	SINGLE(SoundMgr)
private:
	//============================================================
	// NOTE(26-04-09):
	// ASound도 하나의 에셋 객체로
	// BGM, SFX로 묶어서 관리하려면 FMOD 내부를 건드리는게 아닌
	// SoundMgr에서 ASound를 묶어서 관리하고, 원하는 동작을 구현합니다.
	// ex) 전체 BGM mute, 전체 SFX mute
	//============================================================
	map<SOUND_TYPE, map<wstring, Ptr<ASound>>>			m_SoundGroups;

	bool												m_isBGMMute;
	bool												m_isSFXMute;
	float												m_BGMVolume;
	float												m_SFXVolume;

	// 이전 볼륨을 저장하여 뮤트 해제 시 복원하기 위한 변수
	float											m_prevBGMVolume;
	float											m_prevSFXVolume;

	// 뮤트 상태에서 슬라이더로 볼륨을 변경했는지 추적
	bool											m_bBGMChangedWhileMuted;
	bool											m_bSFXChangedWhileMuted;

public:
	void Init();
	void Progress();

	void PlayBGM(wstring _LevelName);
	void StopPrevBGM();

	void PlaySFX(wstring _SFXName);

	// Overload: play with loop count and overlap control (_iRoopCount: 0 -> loop indefinitely)
	void PlaySFX(wstring _SFXName, int _iRoopCount, bool _bOverlap);

	// Stop all channels for a named SFX
	void StopSFX(wstring _SFXName);

	// BGM 일괄 제어
	void SetBGMVolume(float _f);
	void SetBGMMute(bool _bMute);
	bool IsBGMMute() const;

	// Set using 0..100 integer UI value
	void SetBGMVolume_UI(int _iPercent) { SetBGMVolume(_iPercent / 100.f); }
	int GetBGMVolume_UI() const { return (int)(m_BGMVolume * 100.f); }

	// SFX 일괄 제어 (옵션 창에서 사용 가능하도록 추가)
	void SetSFXVolume(float _f);
	void SetSFXMute(bool _bMute);
	bool IsSFXMute() const;

	// Get current global volumes
	float GetBGMVolume() const { return m_BGMVolume; }
	float GetSFXVolume() const { return m_SFXVolume; }

	// UI friendly access (0..100 integer)
	void SetSFXVolume_UI(int _iPercent) { SetSFXVolume(_iPercent / 100.f); }
	int GetSFXVolume_UI() const { return (int)(m_SFXVolume * 100.f); }
};
