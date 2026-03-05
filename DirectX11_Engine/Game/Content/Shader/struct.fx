#ifndef _STRUCT
#define _STRUCT

// Light2D 정보
struct Light2DInfo
{
    int         Type;
    float3      Color;          // 빛의 색상
    float3      Ambient;        // 환경광, 광원이 존재하면서 최소한으로 발생하는 빛의 세기
						        // (현실의 빛의 원리를 구현하기 위함, 현실에서는 빛이 적은 상황이라도 물체가 보이기 때문)
    float3      LightDir;       // 광원의 빛이 향하는 방향
    float3      WorldPos;       // 광원의 위치(Point, SpotLight에서만 사용)
    float       Radius;         // 빛의 영향 반경(Point, SpotLight에서만 사용)
    float       Angle;          // 빛의 각도(SpotLight에서만 사용)
};



#endif