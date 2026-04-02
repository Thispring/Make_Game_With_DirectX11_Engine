#include "pch.h"
#include "CollisionMgr.h"
#include "AssetMgr.h"

CollisionMgr::CollisionMgr()
{
}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::CollisionBtwLayer(Layer* _Left, Layer* _Right)
{
	const vector<Ptr<GameObject>>& vecLeft = _Left->GetAllObjects();
	const vector<Ptr<GameObject>>& vecRight = _Right->GetAllObjects();

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		// 충돌하는 모든 오브젝트에 Collider가 있을 때만 검사
		if (vecLeft[i]->Collider2D() == nullptr)
			continue;

		for (size_t j = 0; j < vecRight.size(); j++)
		{
			// 충돌하는 모든 오브젝트에 Collider가 있을 때만 검사
			if (vecRight[j]->Collider2D() == nullptr)
				continue;

			// 두 충돌체의 고유 ID 로 조합을한 키값 생성
			COL_ID colid;
			colid.LeftID = vecLeft[i]->Collider2D()->GetID();
			colid.RightID = vecRight[j]->Collider2D()->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapColID.find(colid.ID);
	
			if (iter == m_mapColID.end())
			{
				m_mapColID.insert(make_pair(colid.ID, false));
				iter = m_mapColID.find(colid.ID);
			}

			// 충돌 검사를 진행하는 두 오브젝트 중에서 하나라도 Dead 상태인지를 체크
			bool IsDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			// 현재 충돌 중임
			if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// 둘 중 하나가 곧 삭제라면
				// 양쪽 충돌체의 EndOverlap 호출
				if (IsDead)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}
				// 이전에도 충돌했었는지 검사
				else if (iter->second)
				{
					vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
				}
				// 이전에는 충돌하지 않았음
				else
				{
					vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
				}

				iter->second = true;
			}
			// 현재 충돌 중이 아님
			else
			{
				// 이전 프레임에는 충돌 중이었다.
				if (iter->second)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}

				iter->second = false;
			}

			//// 검사하려면 모든 오브젝트에 Collider가 있는지 검사
			//if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			//{
			//	vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
			//	vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
			//	
			//	// 만약 Left 충돌체의 GameObject Layer번호가 8번이라면, 다른 오버랩 호출?
			//	// Layer 번호 4(무기), 8(플레이어의 자식 오브젝트) 끼리 충돌했다면
			//	// EndOverlap을 호출
			//	if (vecLeft[i]->GetLayerIdx() == 4 && vecRight[j]->GetLayerIdx() == 8)
			//	{
			//		// 특정 Layer끼리의 충돌일 시 
			//		// EndOverlap을 호출 시키고, 로직작성
			//		vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
			//	}
			//}

		}
	}
}

bool CollisionMgr::IsCollision(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol)
{
	if (!_LeftCol->IsEnabled() || !_RightCol->IsEnabled())
		return false;

	COLLIDER2D_SHAPE lType = _LeftCol->GetShape();
	COLLIDER2D_SHAPE rType = _RightCol->GetShape();

	if (lType == COLLIDER2D_SHAPE::SECTOR)
		return IsCollisionSectorVsAny(_LeftCol, _RightCol);
	if (rType == COLLIDER2D_SHAPE::SECTOR)
		return IsCollisionSectorVsAny(_RightCol, _LeftCol);

	if (lType == COLLIDER2D_SHAPE::LARGE_BASE_CONE)
		return IsCollisionLargeBaseConeVsAny(_LeftCol, _RightCol);
	if (rType == COLLIDER2D_SHAPE::LARGE_BASE_CONE)
		return IsCollisionLargeBaseConeVsAny(_RightCol, _LeftCol);

	if (lType == COLLIDER2D_SHAPE::CIRCLE && rType == COLLIDER2D_SHAPE::CIRCLE)
		return IsCollisionCircleVsCircle(_LeftCol, _RightCol);

	if (lType == COLLIDER2D_SHAPE::RECT && rType == COLLIDER2D_SHAPE::CIRCLE)
		return IsCollisionOBBvsCircle(_LeftCol, _RightCol);
	if (lType == COLLIDER2D_SHAPE::CIRCLE && rType == COLLIDER2D_SHAPE::RECT)
		return IsCollisionOBBvsCircle(_RightCol, _LeftCol);

	// Both RECT
	return IsCollisionOBBvsOBB(_LeftCol, _RightCol);
}

bool CollisionMgr::IsCollisionOBBvsOBB(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol)
{
	Ptr<AMesh> pRectMesh = FIND(AMesh, L"SquareMesh");

	const Vtx* pVtx = pRectMesh->GetVtxSysMem();

	const Matrix& matWorldLeft = _LeftCol->GetWorldMat();
	const Matrix& matWorldRight = _RightCol->GetWorldMat();

	Vec3 Axis[4] = {};
	Axis[0] = XMVector3TransformCoord(pVtx[1].vPos, matWorldLeft) - XMVector3TransformCoord(pVtx[0].vPos, matWorldLeft);
	Axis[1] = XMVector3TransformCoord(pVtx[3].vPos, matWorldLeft) - XMVector3TransformCoord(pVtx[0].vPos, matWorldLeft);
	Axis[2] = XMVector3TransformCoord(pVtx[1].vPos, matWorldRight) - XMVector3TransformCoord(pVtx[0].vPos, matWorldRight);
	Axis[3] = XMVector3TransformCoord(pVtx[3].vPos, matWorldRight) - XMVector3TransformCoord(pVtx[0].vPos, matWorldRight);

	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldRight) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldLeft);

	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProjAxis = Axis[i];
		vProjAxis.Normalize();

		float Dot = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			Dot += fabs(vProjAxis.Dot(Axis[j]));
		}
		Dot /= 2.f;

		float fCenter = fabs(vCenter.Dot(vProjAxis));

		if (fCenter > Dot)
			return false;
	}

	return true;
}

bool CollisionMgr::IsCollisionCircleVsCircle(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol)
{
	float fDist = Vec3::Distance(_LeftCol->GetWorldCenter(), _RightCol->GetWorldCenter());
	return fDist <= (_LeftCol->GetWorldRadius() + _RightCol->GetWorldRadius());
}

bool CollisionMgr::IsCollisionOBBvsCircle(Ptr<CCollider2D> _OBB, Ptr<CCollider2D> _Circle)
{
	Matrix matInverse = _OBB->GetWorldMat().Invert();
	Vec3 vLocalCircle = XMVector3TransformCoord(_Circle->GetWorldCenter(), matInverse);

	float rx = 0.5f;
	float ry = 0.5f;

	Vec3 vClosest = vLocalCircle;
	if (vClosest.x < -rx) vClosest.x = -rx;
	else if (vClosest.x > rx) vClosest.x = rx;
	if (vClosest.y < -ry) vClosest.y = -ry;
	else if (vClosest.y > ry) vClosest.y = ry;

	Vec3 vWorldClosest = XMVector3TransformCoord(vClosest, _OBB->GetWorldMat());
	float fDist = Vec3::Distance(vWorldClosest, _Circle->GetWorldCenter());

	return fDist <= _Circle->GetWorldRadius();
}

bool CollisionMgr::IsPointInSector(Vec3 _Point, Vec3 _Tip, Vec3 _Dir, float _Radius, float _HalfAngle)
{
	Vec3 vToPoint = _Point - _Tip;
	vToPoint.z = 0.f; // 2D only
	float fDist = vToPoint.Length();

	if (fDist > _Radius) return false;
	if (fDist < 0.0001f) return true; // near tip

	vToPoint.Normalize();
	float fDot = _Dir.Dot(vToPoint);
	return fDot >= cosf(_HalfAngle);
}

bool CollisionMgr::IsCollisionSectorVsAny(Ptr<CCollider2D> _Sector, Ptr<CCollider2D> _Target)
{
	Vec3 vTip = _Sector->GetWorldCenter();
	Vec3 vDir = _Sector->GetWorldDir();
	float fRadius = _Sector->GetWorldRadius();
	float fHalfAngle = _Sector->GetHalfAngle();

	if (_Target->GetShape() == COLLIDER2D_SHAPE::CIRCLE)
	{
		return IsPointInSector(_Target->GetWorldCenter(), vTip, vDir, fRadius, fHalfAngle) ||
			Vec3::Distance(vTip, _Target->GetWorldCenter()) <= _Target->GetWorldRadius();
	}

	if (_Target->GetShape() == COLLIDER2D_SHAPE::RECT || _Target->GetShape() == COLLIDER2D_SHAPE::SECTOR)
	{
		if (IsPointInSector(_Target->GetWorldCenter(), vTip, vDir, fRadius, fHalfAngle))
			return true;

		if (_Target->GetShape() == COLLIDER2D_SHAPE::RECT)
		{
			Matrix mat = _Target->GetWorldMat();
			Vec3 corners[4];
			corners[0] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), mat);
			corners[1] = XMVector3TransformCoord(Vec3(-0.5f,  0.5f, 0.f), mat);
			corners[2] = XMVector3TransformCoord(Vec3( 0.5f, -0.5f, 0.f), mat);
			corners[3] = XMVector3TransformCoord(Vec3( 0.5f,  0.5f, 0.f), mat);
			for (int i = 0; i < 4; ++i)
			{
				if (IsPointInSector(corners[i], vTip, vDir, fRadius, fHalfAngle))
					return true;
			}

			Matrix matInverse = mat.Invert();
			Vec3 vLocalTip = XMVector3TransformCoord(vTip, matInverse);
			if (vLocalTip.x >= -0.5f && vLocalTip.x <= 0.5f && vLocalTip.y >= -0.5f && vLocalTip.y <= 0.5f) return true;
		}
	}

	return false;
}

bool CollisionMgr::IsCollisionLargeBaseConeVsAny(Ptr<CCollider2D> _LBC, Ptr<CCollider2D> _Target)
{
	Matrix matInverse = _LBC->GetWorldMat().Invert();
	Vec3 vLocalCenter = XMVector3TransformCoord(_Target->GetWorldCenter(), matInverse);

	float fTargetRadius = 0.f;
	if (_Target->GetShape() == COLLIDER2D_SHAPE::CIRCLE)
	{
		Vec3 vRadTest = XMVector3TransformNormal(Vec3(_Target->GetWorldRadius(), 0, 0), matInverse);
		fTargetRadius = vRadTest.Length();
	}

	constexpr float fLbcRadiusX = LargeBaseConeConst::RadiusX;
	constexpr float fLbcRadiusY = LargeBaseConeConst::RadiusY;
	float fLbcHalfAngle = _LBC->GetHalfAngle();

	float x = vLocalCenter.x;
	float y = vLocalCenter.y;

	if (_Target->GetShape() == COLLIDER2D_SHAPE::CIRCLE)
	{
		float angle = atan2f(fabs(x), y); 
		if (angle > fLbcHalfAngle && y < 0.f)
			return false;

		float distSq = (x * x) / (fLbcRadiusX * fLbcRadiusX) + (y * y) / (fLbcRadiusY * fLbcRadiusY);
		float ext = 1.f + (fTargetRadius / fLbcRadiusY); 

		if (distSq <= ext * ext && y >= -fTargetRadius)
			return true;
	}
	else if (_Target->GetShape() == COLLIDER2D_SHAPE::RECT || _Target->GetShape() == COLLIDER2D_SHAPE::SECTOR || _Target->GetShape() == COLLIDER2D_SHAPE::LARGE_BASE_CONE)
	{
		float angle = atan2f(fabs(x), y); 
		if (angle <= fLbcHalfAngle && ((x*x)/(fLbcRadiusX*fLbcRadiusX) + (y*y)/(fLbcRadiusY*fLbcRadiusY)) <= 1.f)
			return true;

		if (_Target->GetShape() == COLLIDER2D_SHAPE::RECT)
		{
			Matrix matTargetLocal = _Target->GetWorldMat() * matInverse;
			Vec3 corners[4];
			corners[0] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), matTargetLocal);
			corners[1] = XMVector3TransformCoord(Vec3(-0.5f,  0.5f, 0.f), matTargetLocal);
			corners[2] = XMVector3TransformCoord(Vec3( 0.5f, -0.5f, 0.f), matTargetLocal);
			corners[3] = XMVector3TransformCoord(Vec3( 0.5f,  0.5f, 0.f), matTargetLocal);

			for(int i=0; i<4; ++i)
			{
				float cx = corners[i].x;
				float cy = corners[i].y;
				float a = atan2f(fabs(cx), cy);
				if (a <= fLbcHalfAngle && ((cx*cx)/(fLbcRadiusX*fLbcRadiusX) + (cy*cy)/(fLbcRadiusY*fLbcRadiusY)) <= 1.f)
					return true;
			}

			// 중심 처리도 RECT의 경우
			Vec3 lc = XMVector3TransformCoord(Vec3(0,0,0), matTargetLocal);
			float lc_a = atan2f(fabs(lc.x), lc.y);
			if (lc_a <= fLbcHalfAngle && ((lc.x*lc.x)/(fLbcRadiusX*fLbcRadiusX) + (lc.y*lc.y)/(fLbcRadiusY*fLbcRadiusY)) <= 1.f)
				return true;
		}
	}

	return false;
}

void CollisionMgr::Progress(Ptr<ALevel> _Level)
{
	UINT* pMatrix = _Level->GetCollisionMatrix();

	for (UINT Row = 0; Row < MAX_LAYER; Row++)
	{
		for (UINT Col = Row; Col < MAX_LAYER; Col++)
		{
			if ((pMatrix[Row] & (1 << Col)) == false)
				continue;

			CollisionBtwLayer(_Level->GetLayer(Row), _Level->GetLayer(Col));
		}
	}
}
