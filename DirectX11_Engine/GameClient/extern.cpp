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
	"LEVEL",
	"SPRITE",
	"FLIPBOOK",
	"TILEMAP",
	"PREFAB",
};

const char* ToString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}