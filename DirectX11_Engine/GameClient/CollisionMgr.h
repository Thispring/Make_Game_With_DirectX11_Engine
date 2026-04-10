#pragma once
#include "ALevel.h"
#include "Layer.h"
#include "CCollider2D.h"
#include <unordered_map>

union COL_ID
{
	struct
	{
		UINT	LeftID;
		UINT	RightID;
	};
	ULONGLONG	ID;
};

// 충돌 관리 매니저
class CollisionMgr
	: public Singleton<CollisionMgr>
{
	SINGLE(CollisionMgr);
private:
	map<ULONGLONG, bool>	m_mapColID;

	//=================
	// private 멤버 함수
	//=================
	void CollisionBtwLayer(Layer* _Left, Layer* _Right);
	bool IsCollision(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol);

	bool IsCollisionOBBvsOBB(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol);
	bool IsCollisionCircleVsCircle(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol);
	bool IsCollisionOBBvsCircle(Ptr<CCollider2D> _OBB, Ptr<CCollider2D> _Circle);
	bool IsCollisionSectorVsAny(Ptr<CCollider2D> _Sector, Ptr<CCollider2D> _Target);
	bool IsCollisionLargeBaseConeVsAny(Ptr<CCollider2D> _LBC, Ptr<CCollider2D> _Target);

	static bool IsPointInSector(Vec3 _Point, Vec3 _Tip, Vec3 _Dir, float _Radius, float _HalfAngle);

public:
	//=========
	// 멤버 함수
	//=========
	void Progress(Ptr<ALevel> _Level);
};
