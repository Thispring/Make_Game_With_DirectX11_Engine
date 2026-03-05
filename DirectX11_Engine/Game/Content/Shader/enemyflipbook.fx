#ifndef _EFLIPBOOK
#define _EFLIPBOOK

#include "value.fx"

// (26-02-23): 재질 복사 관련 변수
#define TintColor g_vec4_0

#define AtlasTex    g_tex_0
#define LeftTopUV   g_vec2_0
#define SliceUV     g_vec2_1


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

VS_OUT VS_Flipbook(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;
             
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
     
    output.vPosition = vProj;
    output.vWorldPos = vWorld;
    output.vUV = _input.vUV;
    
    return output;
}

// 입력된 텍스쳐를 사용해서 픽셀쉐이더의 출력 색상으로 지정한다.
float4 PS_Flipbook(VS_OUT _input) : SV_Target
{
    float2 vSpriteUV = _input.vUV * SliceUV + LeftTopUV;
    float4 vColor = AtlasTex.Sample(g_sam_1, vSpriteUV);
    
    if (vColor.a == 0.f)
        discard;
    
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
    
    // 초록색을 더함
    vColor.g += 255.f;
    
    // TakeDamage
    // g_int_0에 1이 들어가면 빨간색으로 변경
    if (g_int_0 == 1)
    {
        vColor.r = 2.f;
    }
    else
    {
        vColor.r = 0.f;
    }
    
    return vColor;
}


#endif