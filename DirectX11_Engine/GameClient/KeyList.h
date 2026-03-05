#pragma once
#include "EditorUI.h"

// 게임 엔진에서 사용되고 있는 키 입력을 텍스트로 저장하여
// 보여주는 UI 입니다.
// Key_info.txt 파일에 있는 문자열을 읽어, ImGui에 표시합니다.
class KeyList :
    public EditorUI
{

private:
    ifstream    m_fopen;        // 파일 읽기 
    vector<string> m_keyList;   // 파일에서 읽어온 문자를 저장하는 멤버

public:
    //=========
    // 멤버 함수
    //=========
    void GetStringFromFile();


    //=============
    // 상속 멤버 함수
    //=============
    virtual void Tick_UI() override;


    //============
    // 생성, 소멸자
    //============
    KeyList();
    virtual ~KeyList();
};
