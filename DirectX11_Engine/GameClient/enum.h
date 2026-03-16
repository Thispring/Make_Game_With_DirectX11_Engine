#pragma once
// 자주 사용되는 enum에 대해 정의합니다.
// 목적에 따라 enum과 enum class를 구분해 사용합니다.


// 상수 버퍼 타입
enum class CB_TYPE
{
	TRANSFORM, // b0
	MATERIAL,  // b1
	GLOBAL,	   // b2

	END
};

// Rasterizer State 타입
enum class RS_TYPE
{
	CULL_BACK,	// 뒷면 (정점 반시계 방향), nullptr과 대응
	CULL_FRONT,	// 앞면
	CULL_NONE,	// Culling X
	WIRE_FRAME,	// 정점과 정점 사이를 잇는 부분을 제외
	END
};

// Debug Shader 타입
enum class DS_TYPE
{
	LESS,				// == nullptr, 기본옵션, 다음 렌더의 깊이가 가까워야 통과
	LESS_EQUL,			// 깊이가 작거나 같아야 통과

	NO_TEST,			// 깊이판정 X, 깊이값 쓰기 O
	NO_TEST_NO_WRITE,	// 깊이판정 X, 깊이값 쓰기 X

	END
};

// Debug Render 모양
enum class DBG_SHAPE
{
	RECT,
	SQUARE,
	CIRCLE,
	CUBE,
	SPHERE,
};

// Blend State 타입
enum class BS_TYPE
{
	DEFAULT,		// (SrcRGB * 1) + (DestRGB * 0)
	ALPHABLEND,		// (SrcRGB * SrcA) + (DestRGB * (1 - SrcA))
	ONE_ONE,		// (ScrRGB * 1) + (DestRGB * 1)
	END
};

// Asset 타입
enum class ASSET_TYPE
{
	MESH,
	MATERIAL,
	TEXTURE,
	SOUND,
	GRAPHICSHADER,
	COMPUTESHADER,
	SPRITE,
	FLIPBOOK,
	TILEMAP,
	PREFAB,
	LEVEL,

	END
};

// Component 타입
enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,

	COLLIDER2D,
	COLLIDER3D,
	LIGHT2D,
	LIGHT3D,

	MESHRENDER,
	SPRITE_RENDER,
	BILLBOARD_RENDER,
	FLIPBOOK_RENDER,
	PARTICLE_RENDER,
	TILE_RENDER,

	END,

	// 하나의 오브젝트가 여러개의 Script를 가질 수 있도록
	// 배열의 끝에 배치하고, Vector 자료형 활용
	SCRIPT,	// 유니티 Script Component와 유사
};

// 방향 개념에 대한 enum
enum class DIR
{
	RIGHT,
	UP,
	FRONT,

	END
};

// Texture 파라미터, .fx에 활용
enum TEX_PARAM
{
	TEX_0,	// t0
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_END,
};

// Scalar 파라미터, .fx에 활용
enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
};

// TaskMgr의 처리할 일의 타입
enum class TASK_TYPE
{
	CREATE_OBJECT,
	DESTROY_OBJECT,
	CHANGE_LEVEL,
	CHANGE_LEVEL_STATE,
};

// 광원 타입
// (현실의 물리법칙에 기반한 빛의 원리와는 다름, 컴퓨터의 랜더링 특성을 고려해 최적화)
enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성, 전역 광원 - 멀리서부터 오는 광원에게 설정(빛이 월드로 오는 방향이 모두 같다고 본다. EX)현실의 태양)
	POINT,			// 점광원 - 기본적인 광원(과학적으로 보았을 때, 모든 빛은 점광원의 특성을 가진다.), 한 점으로부터 주변으로 빛이 뻗어나감
	SPOT,			// 스포트라이트 - 특수한 광원, 빛을 특정 방향으로 모아서 보내는 것으로 봄
};

// 재질이 렌더링 되는 시점
enum class RENDER_DOMAIN
{
	// windows 헤더에 중복된 define이 있어서
	// DOMAIN_NAME 으로 enum class 작명하기

	DOMAIN_OPAQUE,			// 불투명
	DOMAIN_MASKED,			// 불투명 + 완전 투명
	DOMAIN_TRANSPARENT,		// 반투명
	DOMAIN_POSTPROCESS,		// 후처리 - 가장 마지막에 동작, 기존에 그려진 장면을 재 가공

	DOMAIN_DEBUG,			// 디버그 렌더링에 사용하는 재질

	DOMAIN_NONE,			// 미지정
};

// Engine에서 Level의 상태
enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};
