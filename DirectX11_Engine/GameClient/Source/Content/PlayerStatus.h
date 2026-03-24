#pragma once
// NOTE(26-03-23): PlayerStatus를 FSM 설계방식으로 변경합니다.
// Player의 상세한 상태를 이 클래스에서 파생한 자식 클래스로 구현하고, 
// 상태 변경을 확인하여 해당 자식의 함수 ex)Enter, Tick 을 호출합니다.
// 자식의 호출함수에서 구체적인 동작을 정의합니다.
//
// PlayerStatus는 추상 클래스 입니다.

class PlayerStatus 
{

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;

    virtual void Begin() = 0;
    virtual void Tick() = 0;
    virtual void FinalTick() = 0;
    //virtual const char* GetFlipbookName() const = 0;
    virtual int GetFlipbookIndex() = 0;

    //============
    // 생성, 소멸자
    //============
    PlayerStatus();
    virtual ~PlayerStatus();

};
