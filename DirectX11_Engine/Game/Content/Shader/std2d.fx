#ifndef _STD2D   // 중복헤더참조 방지
#define _STD2D   // _TEST가 정의 되어 있지 않으면, #endif 까지의 내용을 실행
                 // 정의 되어 있다면 #endif로 빠져나옴

#include "value.fx"

#define FILL g_float_0
#define TintColor g_vec4_0

// FLOAT_1 비교용 epsilon (부동소수점 오차 방지)
#define FADE_EPS 1e-4f

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
    output.vWorld = vWorld;

    return output;
}

// Pixel Shader 설정
float4 PS_Std2D(VS_OUT _input) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_1, _input.vUV);
    
    // 초록색 배경을 추출
    if (vColor.r == 0.f && vColor.b == 0.f && vColor.g >= 1.f)
    {
        discard;
    }
    
    // 투명 배경 제거
    if (vColor.a == 0.f)
    {
        discard;
    }

    // HP 프로그레스 바 Fill:
    if (FILL > 0.f && _input.vUV.x > FILL)
    {
        discard;
    }

    // 광원 적용        
    float3 LightColor = float3(0.f, 0.f, 0.f);
    for (int i = 0; i < Light2DCount; ++i)
    {
        LightColor += CalcLight2D(i, _input.vWorld);
    }
    vColor.rgb *= LightColor;

    // Fade 처리: VEC2_0 사용
    // g_vec2_0.x : 활성 플래그 (값 > FADE_EPS 이면 페이드 적용)
    // g_vec2_0.y : 경과 시간(초)
    if (g_vec2_0.x > FADE_EPS)
    {
        float fadeDur = 3.0f;
        float fadeHalf = fadeDur * 0.5f;
        float t = g_vec2_0.y;
        float alphaFade = 0.0f;

        if (t > 0.0f && t < fadeDur)
        {
            if (t <= fadeHalf)
            {
                alphaFade = t / fadeHalf; // 0 -> 1 (페이드인)
            }
            else
            {
                alphaFade = (fadeDur - t) / fadeHalf; // 1 -> 0 (페이드아웃)
            }
        }
        else
        {
            // t <= 0 또는 t >= fadeDur 이면 알파는 0 (완전 투명)
            alphaFade = 0.0f;
        }

        vColor.a *= alphaFade;
        vColor.rgb *= alphaFade;

        if (vColor.a <= 0.0f)
            discard;
    }
    // else: 플래그가 없으면 페이드 로직 적용하지 않음(기본 렌더링)

    return vColor;
}


#endif