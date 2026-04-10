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
	// Dead/Collider 없는 오브젝트 미리 필터링
	std::vector<Ptr<GameObject>> filteredLeft, filteredRight;
	for (auto& obj : _Left->GetAllObjects()) {
		if (obj.Get() && obj->Collider2D().Get() && !obj->IsDead())
			filteredLeft.push_back(obj);
	}
	for (auto& obj : _Right->GetAllObjects()) {
		if (obj.Get() && obj->Collider2D().Get() && !obj->IsDead())
			filteredRight.push_back(obj);
	}

	for (size_t i = 0; i < filteredLeft.size(); i++) {
		for (size_t j = 0; j < filteredRight.size(); j++) {
			COL_ID colid;
			colid.LeftID = filteredLeft[i]->Collider2D()->GetID();
			colid.RightID = filteredRight[j]->Collider2D()->GetID();

			auto iter = m_mapColID.find(colid.ID);
			if (iter == m_mapColID.end()) {
				m_mapColID.insert(std::make_pair(colid.ID, false));
				iter = m_mapColID.find(colid.ID);
			}

			if (IsCollision(filteredLeft[i]->Collider2D(), filteredRight[j]->Collider2D())) {
				if (iter->second) {
					filteredLeft[i]->Collider2D()->Overlap(filteredRight[j]->Collider2D());
					filteredRight[j]->Collider2D()->Overlap(filteredLeft[i]->Collider2D());
				} else {
					filteredLeft[i]->Collider2D()->BeginOverlap(filteredRight[j]->Collider2D());
					filteredRight[j]->Collider2D()->BeginOverlap(filteredLeft[i]->Collider2D());
				}
				iter->second = true;
			} else {
				if (iter->second) {
					filteredLeft[i]->Collider2D()->EndOverlap(filteredRight[j]->Collider2D());
					filteredRight[j]->Collider2D()->EndOverlap(filteredLeft[i]->Collider2D());
				}
				iter->second = false;
			}
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
			// 9개 샘플링 포인트 정의
			Vec3 testPoints[9] = {
				Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f,  0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f), Vec3(0.5f,  0.5f, 0.f),
				Vec3(0.f,   0.f,  0.f), Vec3(-0.5f,  0.f,  0.f),
				Vec3(0.5f,  0.f,  0.f), Vec3(0.f,  -0.5f, 0.f),
				Vec3(0.f,   0.5f, 0.f)
			};

			for (int i = 0; i < 9; ++i)
			{
				Vec3 worldPt = XMVector3TransformCoord(testPoints[i], mat);
				if (IsPointInSector(worldPt, vTip, vDir, fRadius, fHalfAngle))
					return true;
			}

			// 역방향 검사: 섹터의 팁이 사각형 안에 있는지 확인
			Matrix matInverse = mat.Invert();
			Vec3 vLocalTip = XMVector3TransformCoord(vTip, matInverse);
			if (fabs(vLocalTip.x) <= 0.5f && fabs(vLocalTip.y) <= 0.5f) return true;
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
		// 1. 타겟의 중심점이 원뿔 안에 있는지 기본 검사
		float angle = atan2f(fabs(x), y);
		if (angle <= fLbcHalfAngle && ((x * x) / (fLbcRadiusX * fLbcRadiusX) + (y * y) / (fLbcRadiusY * fLbcRadiusY)) <= 1.f)
			return true;

		if (_Target->GetShape() == COLLIDER2D_SHAPE::RECT)
		{
			// 2. RECT일 경우 9개 점 샘플링 검사 (꼭짓점 4 + 변 중앙 4 + 중심 1)
			Matrix matTargetLocal = _Target->GetWorldMat() * matInverse;
			Vec3 testPoints[9] = {
				Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f,  0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f), Vec3(0.5f,  0.5f, 0.f),
				Vec3(0.f,   0.f,  0.f), Vec3(-0.5f,  0.f,  0.f),
				Vec3(0.5f,  0.f,  0.f), Vec3(0.f,  -0.5f, 0.f),
				Vec3(0.f,   0.5f, 0.f)
			};

			for (int i = 0; i < 9; ++i)
			{
				Vec3 pt = XMVector3TransformCoord(testPoints[i], matTargetLocal);
				float cx = pt.x;
				float cy = pt.y;
				float a = atan2f(fabs(cx), cy);
				if (a <= fLbcHalfAngle && ((cx * cx) / (fLbcRadiusX * fLbcRadiusX) + (cy * cy) / (fLbcRadiusY * fLbcRadiusY)) <= 1.f)
					return true;
			}

			// 3. 역방향 검사: 원뿔의 원점(Tip)이 RECT 내부에 있는지 확인
			Matrix matConeInTarget = _LBC->GetWorldMat() * _Target->GetWorldMat().Invert();
			Vec3 vConeTipInTarget = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matConeInTarget);

			if (fabs(vConeTipInTarget.x) <= 0.5f && fabs(vConeTipInTarget.y) <= 0.5f)
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
