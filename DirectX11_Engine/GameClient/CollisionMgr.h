#pragma once
#include "ALevel.h"
#include "Layer.h"
#include "CCollider2D.h"

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

public:
	//=========
	// 멤버 함수
	//=========
	void Progress(Ptr<ALevel> _Level);
};
