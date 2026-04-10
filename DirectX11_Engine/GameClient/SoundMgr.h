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

public:
	void Init();
	void Progress();

	void PlayBGM(wstring _LevelName);
	void StopPrevBGM();

	void PlaySFX(wstring _SFXName);
};
