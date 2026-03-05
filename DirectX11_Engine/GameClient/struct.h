#pragma once
// 자주 사용되는 구조체들을 정의합니다.


// 정점 정보
struct Vertex
{
	Vec3 vPos;		// 3D 공간상에서 정점의 위치
	Vec2 vUV;		// 정점 위에 이미지를 띄울 경우, 이미지의 어느 지점을 참고하는지에 대한 좌표
	Vec4 vColor;	// 정점 자체의 색상
};
typedef Vertex Vtx;

// 
struct TransformMatrix
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};
// 전역변수 전방선언, 모든 .cpp에서 접근 가능
extern TransformMatrix g_Trans;

// 
struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	mat[2];

	int		IsTex[(UINT)TEX_PARAM::TEX_END];
	int		Padding[2];	
};

// Debug Render에 대한 정보
struct DbgInfo
{
	DBG_SHAPE	Shape;

	Vec3		Pos;
	Vec3		Scale;
	Vec3		Rotation;

	Matrix		matWorld;

	Vec4		Color;
	
	float		Age;		// 디버그 렌더의 유지 시간 (현재 수명)
	float		Life;		// 최대 수명

	bool		DepthTest;
};

// TaskMgr이 처리할 작업 단위
struct TaskInfo
{
	TASK_TYPE Type;
	DWORD_PTR Param_0;
	DWORD_PTR Param_1;
	DWORD_PTR Param_2;
};

// Light2D 정보
struct Light2DInfo
{
	LIGHT_TYPE	Type;
	Vec3		Color;		// 빛의 색상
	Vec3		Ambient;	// 환경광, 광원이 존재하면서 최소한으로 발생하는 빛의 세기
							// (현실의 빛의 원리를 구현하기 위함, 현실에서는 빛이 적은 상황이라도 물체가 보이기 때문)
	Vec3		LightDir;	// 광원의 빛이 향하는 방향
	Vec3		WorldPos;	// 광원의 위치(Point, SpotLight에서만 사용)
	float		Radius;		// 빛의 영향 반경(Point, SpotLight에서만 사용)
	float		Angle;		// 빛의 각도(SpotLight에서만 사용)
};

// 전역적으로 쉐이더에 전달되었으면 좋을 정보 모음
struct GlobalData
{
	Vec2	Resolution;		// 화면 해상도
	int		Light2DCount;	// 2D 광원 개수
	int		Light3DCount;	// 3D 광원 개수
	// 게임 콘텐츠 시간
	float	DeltaTime;		// DeltaTime
	float	Time;			// 흘러간 시간
	// 엔진 Editer 시간
	float	EngineDT;
	float	EngineTime;
};
extern GlobalData g_Global;
