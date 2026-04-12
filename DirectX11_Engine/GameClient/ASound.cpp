#include "pch.h"
#include "ASound.h"

#include "Engine.h"
#include "AssetMgr.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);

ASound::ASound(bool _EngineRes)
	: Asset(ASSET_TYPE::SOUND)
	, m_Sound(nullptr)
	, m_iMaxOverlap(4) // 기본값: 4중첩 허용
{
}

ASound::~ASound()
{
	if (nullptr != m_Sound)
	{
		FMOD_RESULT result = m_Sound->release();
		m_Sound = nullptr;
	}
}

int ASound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 중첩재생 금지 + 이미 재생중이면 실패
	if (!_bOverlap && !m_listChannel.empty())
	{
		return E_FAIL;
	}

	// 중첩 허용이지만 현재 채널이 최대값을 넘는다면
	// 간단한 전략: 가장 오래된(앞에 있는) 채널을 중지(voice-steal)
	if (_bOverlap && (int)m_listChannel.size() >= m_iMaxOverlap)
	{
		// 가장 오래된 채널을 중지하고 리스트에서 제거
		FMOD::Channel* pOld = m_listChannel.front();
		if (pOld)
		{
			pOld->stop();
		}
		m_listChannel.pop_front();
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	FMOD_SYSTEM->playSound(m_Sound, nullptr, false, &pChannel);

	// 재생을 했는데, 재생중인 채널이 없다 --> 실패
	if (nullptr == pChannel)
		return E_FAIL;

	pChannel->setVolume(_fVolume);

	pChannel->setCallback(&CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	// 어떤 채널에서 Sound 가 재생중인지 기록
	m_listChannel.push_back(pChannel);

	int iIdx = -1;
	pChannel->getIndex(&iIdx);

	return iIdx;
}

void ASound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void ASound::SetVolume(float _f, int _iChannelIdx)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}

void ASound::SetVolumeAll(float _f)
{
	for (FMOD::Channel* pChannel : m_listChannel)
	{
		if (pChannel)
		{
			pChannel->setVolume(_f);
		}
	}
}

void ASound::SetMute(bool _bMute)
{
	for (FMOD::Channel* pChannel : m_listChannel)
	{
		if (pChannel)
		{
			pChannel->setMute(_bMute);
		}
	}
}

void ASound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

int ASound::Load(const wstring& _FilePath)
{
	string path(_FilePath.begin(), _FilePath.end());

	FMOD_RESULT result = FMOD_SYSTEM->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sound);
	if (FMOD_OK != result)
	{
		assert(nullptr);
	}

	return S_OK;
}

// 현재 사운드가 재생 중인지 반환
bool ASound::IsPlaying() const
{
    return !m_listChannel.empty();
}

// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	ASound* pSound = nullptr;

	switch (controltype)
	{
	// 사운즈 재생 종료시 발생하는 이벤트
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}