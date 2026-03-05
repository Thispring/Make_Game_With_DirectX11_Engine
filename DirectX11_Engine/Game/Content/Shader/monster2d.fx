#ifndef _MONSTER2D   // 중복헤더참조 방지
#define _MONSTER2D

#include "value.fx"

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
    float3 vWorldPos : POSITION;
};

// Vertex Shader 설정
VS_OUT VS_Std2D(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Local -> World
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    
    // World - > View
    float4 vView = mul(vWorld, g_matView);
    
    // View -> Proj
    float4 vProj = mul(vView, g_matProj);
    
    output.vPosition = vProj;
    output.vUV = _input.vUV;
    output.vColor = _input.vColor;
    output.vWorldPos = vWorld;

    return output;
}

// Pixel Shader 설정
float4 PS_Std2D(VS_OUT _input) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_1, _input.vUV);
    
    // 초록색 배경을 추출
    // (컴퓨터에서는 주로 마젠타 색상이 그린스크린 역할을 합니다.)
    if (vColor.r == 0.f && vColor.b == 0.f && vColor.g >= 1.f)
    {
        discard;
    }
    
    // g_int_0에 1이 들어가면 빨간색으로 변경
    if (g_int_0 == 1)
    {
        vColor.r = 2.f;
    }
    else
    {
        vColor.r = 0.f;
    }
    
    // 광원 적용        
    // 물체가 받는 빛의 총량
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    // 반복문 돌면서, 모든 광원으로부터 어느정도의 빛을 받는지 합산
    for (int i = 0; i < Light2DCount; ++i)
    {
        LightColor += CalcLight2D(i, _input.vWorldPos);
    }
    
    // 물체의 색상에, 자신이 받는 최종빛 총량을 곱한다.
    vColor.rgb *= LightColor;
    
    return vColor;
}


#endif