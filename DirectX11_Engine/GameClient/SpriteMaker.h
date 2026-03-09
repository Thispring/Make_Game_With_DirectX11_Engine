#pragma once
#include "EditorUI.h"

// 원하는 아틀라스 텍스쳐에서 해상도 및 개수를 정하여
// Sprite 파일로 만들어주는 UI 클래스
class SpriteMaker :
    public EditorUI
{

private:

public:

    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    SpriteMaker();
    virtual ~SpriteMaker();
};
