#pragma once
#include "CRenderComponent.h"
#include "AFlipbook.h"

class CFlipbookRender :
    public CRenderComponent
{

private:
    vector<Ptr<AFlipbook>>  m_vecFlipbook;      // 여러개의 Flipbook을 들고 있을 수 있도록 vector로 선언
    int                     m_CurFlipbook;      // 현재 어떤 Flipbook 인지를 나타내는 vector 인덱스
    int                     m_CurSprite;        // Flipbook 안의 Sprite 인덱스
    int                     m_RepeatCount;      // -1: 반복재생, 1 이상: 재생 횟수 EX) 2 -> 2번 재생
    bool                    m_Finish;           // Flipbook 1회 재생이 끝났는지 여부
    float                   m_FPS;              // 애니메이션 프레임 수
    float                   m_AccTime;          // 현재 시간

    int                     m_PrevFlipbook;     // 이전에 재생했던 Flipbook vector 인덱스

    // ImGui에서 재생 컨트롤용으로 사용 중인 bool 변수
    // EFlipbookRenderUI 클래스에 의존하고 있어서 사용 주의 필요
    bool                    m_IsStop;

    //=================
    // private 멤버 함수
    //================= 
    bool CheckFinish();

public:
    //=========
    // 멤버 함수
    //=========
    void AddFlipbook(Ptr<AFlipbook> _Flipbook) { m_vecFlipbook.push_back(_Flipbook); }
    void DeleteFlipbook(int _Idx);
    void Play(int _FlipbookIdx, float _FPS, int _RepeatCount);
    void EditorPlay(int _FlipbookIdx, float _FPS, int _RepeatCount);


    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;

    CLONE(CFlipbookRender);
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    void SetFlipbook(int _Idx, Ptr<AFlipbook> _Flipbook)
    {
        if (m_vecFlipbook.size() <= _Idx)
            m_vecFlipbook.resize(_Idx + 1);
        m_vecFlipbook[_Idx] = _Flipbook;
    }
    bool GetFinish() { return m_Finish; }
    const vector<Ptr<AFlipbook>>& GetVecFlipbook() { return m_vecFlipbook; }
    // 인덱스로 해당 Flipbook 에셋을 반환하는 함수
    const Ptr<AFlipbook>& GetVecFlipbook(int _Idx) { return m_vecFlipbook[_Idx]; }
    GET_SET(int, CurFlipbook);
    GET_SET(int, PrevFlipbook);
    GET_SET(int, RepeatCount);
    GET_SET(float, FPS);
    GET_SET(bool, IsStop);


    //============
    // 생성, 소멸자
    //============
    CFlipbookRender();
    // 기본 복사 생성자 사용
    virtual ~CFlipbookRender();
};
