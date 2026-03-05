#pragma once
#include "Asset.h"
#include "ASprite.h"

// Sprite 이미지를 묶어, 연속으로 재생
class AFlipbook :
    public Asset
{

private:
    vector<Ptr<ASprite>> m_vecSprite;   // 여러 개의 Sprite 이미지를 묶기 위한 vector

public:
    //=========
    // 멤버 함수
    //=========
    void AddSprite(Ptr<ASprite> _Sprite) { m_vecSprite.push_back(_Sprite); };


    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


    //=========
    // Get, Set
    //=========
    // SetSprite는 특정 인덱스를 받아서 설정
    void SetSprite(int _Idx, Ptr<ASprite> _Sprite)
    {
        // vector의 size보다 인덱스가 더 크다면
        // resize 호출
        if (m_vecSprite.size() <= _Idx)
            m_vecSprite.resize(_Idx + 1);

        m_vecSprite[_Idx] = _Sprite;
    }

    Ptr<ASprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }

    UINT GetSpriteCount() { return m_vecSprite.size(); }


    //============
    // 생성, 소멸자
    //============
    AFlipbook();
    virtual ~AFlipbook();
};
