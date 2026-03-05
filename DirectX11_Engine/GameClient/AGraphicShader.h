#pragma once
#include "Asset.h"

/********************************************
* .fx 파일에 전달할 파라미터 열거형과 구조체 선언
* Sampling 단계에서 어떤 값을 전달할지를
* AGraphicShader가 저장하게 합니다.
********************************************/
enum class SHADER_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
    MAT,
    TEX,
};

struct ShaderParam
{
    SHADER_PARAM    Type;       // 어떤 파라미터 타입인지
    int             Index;      // 몇번 Index로 들어갈건지
    wstring         Desc;       // 어떤 역할을 하는지 문자열로 저장
    int             Step;       // UI에서 값의 변화량
    bool            IsInput;    // UI로 표현할 위젯 스타일
};

// Asset으로 설정했다는 것은 프로젝트에서 공유자원 역할을 합니다.
// 렌더링 파이프라인 자체를 하나의 Asset으로 봅니다.
// Asset 상속을 받는 class는 이름 앞에 'A'를 붙입니다.
class AGraphicShader :
    public Asset
{
private:
    ComPtr<ID3DBlob>			m_VSBlob;   // HLSL 로 작성한 VS 함수를 컴파일한 어셈블리코드를 저장시킬 버퍼
    ComPtr<ID3DBlob>			m_PSBlob;   // HLSL 로 작성한 PS 함수를 컴파일한 어셈블리코드를 저장시킬 버퍼

    ComPtr<ID3D11VertexShader>	m_VS;
    ComPtr<ID3D11PixelShader>	m_PS;

    RS_TYPE                     m_RSType;   // 레스터라이저 컬링 모드 설정
    DS_TYPE                     m_DSType;   // 깊이 설정
    BS_TYPE                     m_BSType;   // 블랜딩 공식 설정

    ComPtr<ID3D11InputLayout>	m_Layout;   // 정점 쉐이더에 입력으로 들어오는 정점 하나의 구성정보

    D3D11_PRIMITIVE_TOPOLOGY    m_Topology; // 렌더링 과정에서, 정점들을 어떤 도형으로 인지할 것인지
    
    vector<ShaderParam>         m_vecShaderParam;

public:
    //=========
    // 멤버 함수
    //=========
    int CreateVertexShader(const wstring& _RelativeFilePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativeFilePath, const string& _FuncName);
    void Binding();
    void AddShaderParam(SHADER_PARAM _Type, int _Idx, const wstring& _Desc, int _Step = 0, bool _IsInput = true)
    {
        m_vecShaderParam.push_back(ShaderParam{ _Type, _Idx, _Desc, _Step, _IsInput });
    }
    

    //=========
    // Get, Set
    //=========
    GET_SET(D3D11_PRIMITIVE_TOPOLOGY, Topology);
    GET_SET(RS_TYPE, RSType);
    GET_SET(DS_TYPE, DSType);
    GET_SET(BS_TYPE, BSType);
    const vector<ShaderParam>& GetShaderParam() { return m_vecShaderParam; }
    

    //============
    // 생성, 소멸자
    //============
    AGraphicShader();
    virtual ~AGraphicShader();
};
