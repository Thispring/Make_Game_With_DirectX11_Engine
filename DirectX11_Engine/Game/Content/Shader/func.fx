#ifndef _FUNC
#define _FUNC

#include "value.fx"

float3 CalcLight2D(int _LightIdx, float3 _PixelPos)
{
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    // 광원과 물체의 거리에 따른 빛의 세기 비율
    float DistRatio = 1.f;
    
    // DirectionalLight
    if (0 == g_Light2D[_LightIdx].Type)
    {
        LightColor = g_Light2D[_LightIdx].Color + g_Light2D[_LightIdx].Ambient;
    }
    
    // Point
    else if (1 == g_Light2D[_LightIdx].Type)
    {
        // LightColor
        LightColor = g_Light2D[_LightIdx].Color;
        
        // 광원에서 픽셀을 향하는 방향벡터(월드공간 기준)
        float3 LightToPixel = _PixelPos - g_Light2D[_LightIdx].WorldPos;
        
        // 2차원으로만 거리를 따질 것이기 때문에, z 성분 제거
        LightToPixel.z = 0.f;
        
        // 벡터의 길이 측정(광원과 픽셀 사이의 거리)
        float Dist = length(LightToPixel);
        
        // 물체와 광원의 거리가 멀어질수록 빛의 세기가 0 에 가까워진다.
        //DistRatio = saturate(1.f - (Dist / g_Light2D[_LightIdx].Radius) );                   
        //DistRatio = saturate(cos(Dist * ((PI / 2.f) / g_Light2D[_LightIdx].Radius)));
        DistRatio = saturate(cos(saturate(Dist / g_Light2D[_LightIdx].Radius) * (PI / 2.f)));
    }
    
    // Spot
    else
    {
        // SpotLight는 특정 위치에서 원뿔모양으로 빛을 내보냅니다.
        // Light가 시작하는 위치와, 끝나는 지점까지의 방향 벡터가 필요하고, 원뿔을 나타내기 위해
        // 각도를 알아야 합니다. 설정한 각도에서 각도가 작아질 수록, 빛을 강하게 받으며, 반대로
        // 각도가 커질 수록 빛을 약하게 받습니다. 각도에서 벗어나는 범위는 빛의 세기를 0으로 설정하여
        // 빛이 닿지 않아 보이지 않게 만들어야하며, 각도에서 영향을 받는 범위는 보간을 통해 자연스럽게
        // 빛의 세기가 연결되게 만들어야 합니다.
        
        // LightInfo에서 광원이 시작하는 벡터인 WorldPos와 광원이 빛을 향하는 벡터인 LightDir를 내적하여
        // 내적의 길이에 비례하는 각도와 LightInfo에서 설정한 각도를 비교하여, 설정한 각도보다 크다면 빛을 약하게 받도록
        // 반대로 설정한 각도보다 작을 수록 빛을 강하게 받도록 설정합니다.
        
   
        // LightColor
        LightColor = g_Light2D[_LightIdx].Color;
        
        // 광원에서 픽셀을 향하는 방향벡터(월드공간 기준)
        float3 LightToPixel = _PixelPos - g_Light2D[_LightIdx].WorldPos;
        
        // 2차원으로만 거리를 따질 것이기 때문에, z 성분 제거
        LightToPixel.z = 0.f;
        
        // 벡터의 길이 측정(광원과 픽셀 사이의 거리)
        float Dist = length(LightToPixel);
                
        // SpotLight 각도 범위 이내인지 검사
        // 설정한 각도 안에 들어와있는지 아닌지를 확인
        float3 vLightDir = normalize(g_Light2D[_LightIdx].LightDir);
        float3 vLightToPixel = normalize(LightToPixel);
        float Theta = acos(dot(vLightToPixel, vLightDir));
                
        if (Theta < g_Light2D[_LightIdx].Angle * 0.5f)
            DistRatio = saturate(cos(saturate(Dist / g_Light2D[_LightIdx].Radius) * (PI / 2.f)));
        else    // 범위를 벗어난다면 빛을 받지 않는 곳이므로 0으로 설정
            DistRatio = 0.f;
    }
    
    return LightColor * DistRatio;
}


#endif