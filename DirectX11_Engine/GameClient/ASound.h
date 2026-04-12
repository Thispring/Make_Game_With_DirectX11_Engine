#pragma once
#include "Asset.h"

enum class SOUND_TYPE
{
    BGM,
    SFX,

    END,
};

class ASound :
    public Asset
{

private:
    FMOD::Sound*            m_Sound;        // Sound 객체
    list<FMOD::Channel*>	m_listChannel;  // Sound 가 재생되고 있는 채널 리스트

    // 동시에 생성 가능한 최대 중첩 채널 수 (기본값은 4)
    int                     m_iMaxOverlap;

public:
    //=========
    // 멤버 함수
    //=========
    // _iRoopCount : 0 (반복재생),  _fVolume : 0 ~ 1(Volume), _bOverlap : 같은 사운드를 중첩해서 켤 수 있는지
    int Play(int _iRoopCount, float _fVolume, bool _bOverlap);
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    void Stop();

    // 현재 사운드가 재생 중인지 반환
    bool IsPlaying() const;

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);

    // 모든 채널의 볼륨을 일괄 설정
    void SetVolumeAll(float _f);

    // 모든 채널 음소거/해제
    void SetMute(bool _bMute);

    // 최대 중첩 수 설정 / 조회
    void SetMaxOverlap(int _iMax) { m_iMaxOverlap = max(1, _iMax); }
    int GetMaxOverlap() const { return m_iMaxOverlap; }


    //=============
    // 상속 멤버 함수
    //=============
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override { return S_OK; }
    CLONE(ASound);


    //============
    // 생성, 소멸자
    //============
    ASound(bool _EngineRes = false);
    ~ASound();


    //=============
    // friend class
    //=============
    friend class CAssetMgr;
};
