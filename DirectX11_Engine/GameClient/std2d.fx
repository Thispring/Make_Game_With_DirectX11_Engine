#ifndef _STD2D   // 중복헤더참조 방지
#define _STD2D   // _TEST가 정의 되어 있지 않으면, #endif 까지의 내용을 실행
                 // 정의 되어 있다면 #endif로 빠져나옴

#include "value.fx"

#define TintColor g_vec4_0

struct VS_IN
{
    // 대문자로 설정한 것은 Semantic, Layout에서 설정한 이름
    float4 vColor : COLOR;
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
}; // C++ struct.h의 Vertex 구조체와 유사

struct VS_OUT
{
    float4 vPosition : SV_Position; // 래스터라이져로 보낼 때, NDC 좌표
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
    float3 vWorld : POSITION;
};

// Vertex Shader 설정
VS_OUT VS_Std2D(VS_IN _input)
{
    /**************************************************************************************
    * [Vertex Shader]:
    *
    * Input Assembler 단계에서 받은 정점들의 정보를 공간 좌표계에 그리기 위해
    * 변환하는 과정, 정점들은 자신만의 좌표계인 Local Space의 좌표를 가지는데,
    * 모든 물체들이 하나의 월드에 위치하도록 Local Space에서 World Space로 변환하고,
    * 실제 플레이어가 바라보는 카메라가 중심이 되는 View Space로 변환해주어야 합니다.
    *
    * Local -> 좌표계에서 모델들이 가진 정점 데이터
    * World -> 모든 물체를 하나의 공통된 공간에 배치하기 위해 필요합니다.
    *          Local에서 World 좌표로 변환하는 과정을 거칩니다.
    * View -> World 공간에 배치된 물체들을 카메라(실제 플레이어가 바라보는 시선)의 시점에 바라보도록
    *         변환해야 합니다. 카메라가 바라보는 방향을 기준으로 새로운 좌표계로 모든 정점을 옮깁니다.
    **************************************************************************************/
    VS_OUT output = (VS_OUT) 0.f;
    
    /************************************************************************************
    * (26-01-22)
    *                          ( scale.x     0         0       0)
    * (x, y, z, 동차좌표)  X    (   0      scale.y      0       0)
    *                          (   0         0      scale.z    0)
    *                          (  Pos.x    Pos.y      Pos.x    1) 
    * 동차좌표가 1이나 0 이냐에 따라서, 곱하는 변환행렬의 4행 이동정보를 적용할지 말지가 결정됩니다.
    ************************************************************************************/
    
    // Local -> World
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    
    // World - > View
    float4 vView = mul(vWorld, g_matView);
    
    // View -> Proj
    float4 vProj = mul(vView, g_matProj);
    
    /**********************************************************************************
    * 투영행렬 특성상, 정확한 NDC 좌표를 얻기 위해서
    * 투영행렬을 곱합 View 좌표의 Z값으로 나누눈 작업이 행렬안에 있어야 합니다.
    * 하지만 이것이 불가능하기 때문에 연산결과의 w(4열) 자리에 View 좌표의 Z가 출력되도록 합니다.
    **********************************************************************************/
    
    output.vPosition = vProj;
    output.vUV = _input.vUV;
    output.vColor = _input.vColor;
    output.vWorld = vWorld;
    
    /*************************************************************************************************
    * 투영 행렬을 계산한 결과값의 x, y, z를 w로 나눠야 최종 NDC 좌표를 얻을 수 있습니다.
    * x, y, z를 w로 나누는 작업을 레스터라이저 에서 진행하기 때문에, 수동으로 나누는 코드를 작성할 필요는 없습니다.
    *************************************************************************************************/
    return output;
}

// Pixel Shader 설정
float4 PS_Std2D(VS_OUT _input) : SV_Target
{
    /***************************************************************************************************************
    * [Pixel Shader]:
    *
    * 래스터라이저(Rasterizer)에 의해 계산된 각 픽셀의 위치에 대해, 보간된 데이터(색상, 텍스처 좌표 등)를 입력으로 받습니다. 
    * 이 데이터를 사용해 텍스처 샘플링, 조명 계산 등 복잡한 연산을 수행하고, 해당 픽셀의 최종 색상을 계산하여 출력하는 역할을 합니다.
    * 
    * 보간(Interpolation)에 대한 개념을 꼭 알고 있기
    *
    * 원본 텍스쳐의 알파값을 사용하는 경우
    *  => .png의 투명 배경을 사용
    ***************************************************************************************************************/
    float4 vColor = g_tex_0.Sample(g_sam_1, _input.vUV);
    
    // 초록색 배경을 추출
    // (컴퓨터에서는 주로 마젠타 색상이 그린스크린 역할을 합니다.)
    if (vColor.r == 0.f && vColor.b == 0.f && vColor.g >= 1.f)
    {
        discard;
    }
    
    // 투명 배경 제거
    if (vColor.a == 0.f)
    {
        discard;
    }
    
    // 이 코드가 실행되었을 때, 색상이 검은색으로 출력되는지 확인
    //vColor *= TintColor;
    
    // g_int_0에 1이 들어가면 빨간색으로 변경
    if (g_int_0 == 1)
    {
        //vColor.r *= 2.f;
    }
    
    // 광원 적용        
    // 물체가 받는 빛의 총량
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    // 반복문 돌면서, 모든 광원으로부터 어느정도의 빛을 받는지 합산
    for (int i = 0; i < Light2DCount; ++i)
    {
        LightColor += CalcLight2D(i, _input.vWorld);
    }
    
    // 물체의 색상에, 자신이 받는 최종빛 총량을 곱한다.
    vColor.rgb *= LightColor;
    
    return vColor;
}


#endif