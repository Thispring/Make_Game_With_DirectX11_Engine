#include "pch.h"

// 전역변수 초기화
TransformMatrix g_Trans = {};
GlobalData		g_Global = {};

const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END] =
{
	"MESH",
	"MATERIAL",
	"TEXTURE",
	"SOUND",
	"GRAPHICSHADER",
	"COMPUTESHADER",
	"SPRITE",
	"FLIPBOOK",
	"TILEMAP",
	"PREFAB",
	"LEVEL",
};

const char* ToString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}