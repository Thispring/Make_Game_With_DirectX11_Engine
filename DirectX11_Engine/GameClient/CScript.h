#pragma once
#include "Component.h"
#include "Source/ScriptMgr.h"

// ImGui로 자식 Script들의 멤버를 표시하기 위함
// 필요한 자료형이 있다면 추가합니다.
enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
    MATRIX,

    TEXTURE,
    MATERIAL,
    STRING,     // 포인터 변수 사용 시, 가리키고 있는 대상의 이름 표시
};

// ImGui로 전달할 데이터 구조체
struct tScriptParam
{
    SCRIPT_PARAM    Param;
    void*           Data;
    wstring         Desc;
    bool            IsInput;
    float           Step;
};

class CCollider2D;

class CScript :
    public Component
{

private:
    int                     m_ScriptType;       // enum 값으로 초기화
    vector<tScriptParam>    m_vecScriptParam;

protected:
    // 스크립트 컴포넌트의 오브젝트 삭제 요청 함수
    void Destroy(); 
    // m_vecScriptParam 데이터 push back 함수
    void AddScriptParam(SCRIPT_PARAM _Type, void* _Data, const wstring& _Desc, bool _IsInput = true, float _Step = 1.f);
    // Prefab을 가져와 객체로 생성하는 함수
    void Instantiate(class APrefab* _Prefab, int _LayerIdx, Vec3 _WorldPos);

public:
    //=============
    // 상속 멤버 함수
    //=============
    virtual void Begin() {}
    virtual void Tick() = 0;
    /************************************************************
    * virtual 함수 뒤에 final을 붙이면, 그 가상함수는
    * 더 이상 파생 클래스에서 재정의(override)할 수 없음을 의미합니다.
    ************************************************************/
    virtual void FinalTick() final{}
    virtual CScript* Clone() = 0;


    //=========
    // Get, Set
    //=========
    int GetScriptType() { return m_ScriptType; }
    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }


    //============
    // 생성, 소멸자
    //============
    CScript(int _ScriptType);
    virtual ~CScript();
};

// 자주 사용되는 OverLap 관련 기능 재정의
#define ADD_DYNAMIC_BEGIN_OVERLAP(MemFunc) Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_OVERLAP(MemFunc) Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_END_OVERLAP(MemFunc) Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&MemFunc);
