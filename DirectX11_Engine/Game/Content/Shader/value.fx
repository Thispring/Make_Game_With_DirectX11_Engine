#ifndef _VALUE   // 중복헤더참조 방지
#define _VALUE   

#include "struct.fx"

SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


// HLSL 5.0 기준 4096 의 메모리 공간만 사용 할 수 있다.
// 그렇기에 포인터 형식의 텍스쳐 레지스터를 활용한다. (용량 문제)
cbuffer TRANSFORM : register(b0)
{
    /*************************************************************************
    * 월드 행렬
    * float4x4 = matrix
    * GPU는 데이터를 열기준으로 읽어와 행부터 출력 (row_major는 행부터 읽어오게 명령)
    *************************************************************************/
    row_major matrix g_matWorld;    // Local(Model) -> World Space
    row_major matrix g_matView;     // World -> Camea(View) Space
    row_major matrix g_matProj;     // Camera(View) -> Projection Space
}

cbuffer MATERIAL : register(b1)
{
    // 16 바이트 단위로 인식
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major Matrix g_mat_0;
    row_major Matrix g_mat_1;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int2 padding;
}

// 전역적으로 쉐이더에 전달되었으면 좋을 정보 모음
cbuffer GLOBAL : register(b2)
{
    float2  Resolution;     // 화면 해상도
    int     Light2DCount;   // 2D 광원 개수
    int     Light3DCount;   // 3D 광원 개수
	// 게임 콘텐츠 시간
    float   DeltaTime;      // DeltaTime
    float   Time;           // 흘러간 시간
	// 엔진 Editer 시간
    float   EngineDT;
    float   EngineTime;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

StructuredBuffer<Light2DInfo> g_Light2D : register(t12);

#define PI 3.1415926535f

#include "func.fx"

#endif