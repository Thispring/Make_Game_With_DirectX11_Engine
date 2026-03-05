#pragma once
#include "Asset.h"
#include "AGraphicShader.h"
#include "ATexture.h"

class AMaterial :
    public Asset
{

private:
    Ptr<AGraphicShader>     m_Shader;
    Ptr<ATexture>           m_Tex[TEX_END];
    MtrlConst               m_Const;

    RENDER_DOMAIN           m_Domain;

public:
    //=========
    // 멤버 함수
    //=========
    void Binding();
    void Clear();                           // Material 바인딩 해제 함수
    AMaterial* Clone();                     // Material 임시 복사 함수

    
    //=============
    // 상속 멤버 함수
    //=============
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


    //=========
    // Get, Set
    //=========
    // 값을 전달 받아, 변수 타입에 따라 형 변환하여 재질 상수를 변경시키는 함수
    template<typename T>
    void SetScalar(SCALAR_PARAM _Param, const T& _Data);

    template<typename T>
    T& GetScalar(SCALAR_PARAM _Param);

    Ptr<AGraphicShader> GetShader() { return m_Shader; }
    void SetShader(Ptr<AGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<ATexture> GetTexture(TEX_PARAM _Param) { return m_Tex[_Param]; }
    void SetTexture(TEX_PARAM _Param, Ptr<ATexture> _Texture) { m_Tex[_Param] = _Texture; }
    GET_SET(RENDER_DOMAIN, Domain);


    //============
    // 생성, 소멸자
    //============
    AMaterial();
    virtual ~AMaterial();
};

template<typename T>
void AMaterial::SetScalar(SCALAR_PARAM _Param, const T& _Data)
{
    /*************************************************************************
    * _Param은 enum 값이므로, 설정한 enum 번호와 인덱스가
    * 서로 호환이 가능합니다. 각 변수별로 0번째 자리에 _Data를 역참조하여
    * 값을 넣을 수 있도록 _Param에 각 enum 변수 0번째를 뺀 값으로 인덱스를 사용합니다.
    *************************************************************************/
    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        if constexpr (std::is_same_v<T, int>)
            m_Const.iArr[_Param] = _Data;
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        if constexpr (std::is_same_v<T, float>)
            m_Const.fArr[_Param - FLOAT_0] = _Data;
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        if constexpr (std::is_same_v<T, Vec2>)
            m_Const.v2Arr[_Param - VEC2_0] = _Data;
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        if constexpr (std::is_same_v<T, Vec4>)
            m_Const.v4Arr[_Param - VEC4_0] = _Data;
        break;
    case MAT_0:
    case MAT_1:
        if constexpr (std::is_same_v<T, Matrix>)
            m_Const.mat[_Param - MAT_0] = _Data;
        break;
    }
}

template<typename T>
inline T& AMaterial::GetScalar(SCALAR_PARAM _Param)
{
    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, UINT>)
            return m_Const.iArr[_Param];
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        if constexpr (std::is_same_v<T, float>)
            return m_Const.fArr[_Param - FLOAT_0];
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        if constexpr (std::is_same_v<T, Vec2>)
            return m_Const.v2Arr[_Param - VEC2_0];
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        if constexpr (std::is_same_v<T, Vec4>)
            return m_Const.v4Arr[_Param - VEC4_0];
        break;
    case MAT_0:
    case MAT_1:
        if constexpr (std::is_same_v<T, Matrix>)
            return m_Const.mat[_Param - MAT_0];
        break;
    }
}
