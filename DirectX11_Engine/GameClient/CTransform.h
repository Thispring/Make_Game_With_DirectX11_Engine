#pragma once
#include "Component.h"

class CTransform :
    public Component
{

private:
    Vec3                    m_RelativePos;
    Vec3                    m_RelativeScale;
    Vec3                    m_RelativeRot;

    /*********************************************************************************************
    * 단위벡터(길이가 1, 정규화, 어느 방향인지가 중요하지 해당 방향으로 얼마나 길이가 있는지는 중요하지 않다.)
    * 캐릭터를 움직이기 위해, 향하고 있는 방향으로 좌표값을 더하는 방법을 사용,
    * 3가지 방향만 알고있다면, 나머지방향은 3방향을 음수로 계산하면 반대방향이 됩니다.
    *********************************************************************************************/
    Vec3                    m_Dir[(UINT)DIR::END];  // 방향벡터
    Matrix                  m_matWorld;

    bool                    m_IndependentScale;     // 부모 오브젝트의 크기를 무시할지 여부

public:
    //=========
    // 멤버 함수
    //=========
    void Binding(); // 데이터를 GPU 메모리로 전송

    // 매개변수 만큼 RelativeScale를 곱하는 함수
    void MulRelativeScale(float _Mul) { m_RelativeScale *= _Mul; }


    //=============
    // 상속 멤버 함수
    //=============
    virtual void FinalTick() override;
    
    CLONE(CTransform);
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    //=========
    // Get, Set
    //=========
    Vec3 GetRelativePos() { return m_RelativePos; }
    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3 GetRelativeRot() { return m_RelativeRot; }
    Vec3 GetDir(DIR _Type) { return m_Dir[(UINT)_Type]; }   // 방향정보 반환
    const Matrix& GetWorldMat() { return m_matWorld; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();

    void SetRelativePos(Vec3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeRot(Vec3 _Rot) { m_RelativeRot = _Rot; }
    void SetFrontDir(Vec3 _Dir) { m_Dir[(UINT)DIR::FRONT] = _Dir; }
    // (26-02-22): WorldPos를 Setting 하는 함수 추가
    void SetWorldPos(Vec3 _Pos) { m_matWorld.Translation() = _Pos;}
    void SetWorldMat(const Matrix& _matWorld) { m_matWorld = _matWorld; }
    
    void SetIndependentScale(bool _On) { m_IndependentScale = _On; }
    bool IsIndependentScale() { return m_IndependentScale; }


    //============
    // 생성, 소멸자
    //============
    CTransform();
    // 기본 복사 생성자 사용
    virtual ~CTransform();
};
