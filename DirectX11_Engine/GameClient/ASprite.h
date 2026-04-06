#pragma once
#include "Asset.h"
#include "ATexture.h"

// 이미지 조각
// 하나의 이미지에 모든 동작을 합쳐 놓은 이유는?
// 렌더링 과정에서 연산 최적화를 위해
// 텍스쳐 1장을 쓰는 것이
// 텍스쳐 여러장을 쓰는 것 보다, 연산 및 레지스터 번호 부족 문제에
// 유리하기에 해당 방법을 사용합니다.
// [아틀라스 텍스쳐]
// Sprite는 아틀라스 텍스쳐 전체에서 사용할 하나의 조각을 지칭
// UV 좌표계를 기준으로 얼마나 잘라서 사용할지 계산
class ASprite :
    public Asset
{

private:
    Ptr<ATexture>   m_Atlas;    // 자신이 가리킬 아틀라스 텍스쳐
    
    // UV 좌표 기준으로 얼마큼 자를 지 결정
    Vec2            m_LeftTopUV;
    Vec2            m_SliceUV;

    // 추후 사용될 추가 UV 정보
    Vec2            m_BackgroundUV;     // Texture의 틀 크기
    Vec2            m_OffsetUV;         // Texture 중점

public:
    //=============
    // 상속 멤버 함수
    //=============
    CLONE(ASprite);
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


    //=========
    // Get, Set
    //=========
    GET_SET(Ptr<ATexture>, Atlas);
    GET_SET(Vec2, LeftTopUV);
    GET_SET(Vec2, SliceUV);
    GET_SET(Vec2, BackgroundUV);
    GET_SET(Vec2, OffsetUV);
    

    //============
    // 생성, 소멸자
    //============
    ASprite();
    virtual ~ASprite();
};
