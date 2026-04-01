#include "pch.h"
#include "CCollider2D.h"
#include "RenderMgr.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Shape(COLLIDER2D_SHAPE::RECT)
	, m_HalfAngle(XM_PIDIV4)
	, m_WorldCenter(Vec3(0.f, 0.f, 0.f))
	, m_WorldDir(Vec3(0.f, 1.f, 0.f))
	, m_WorldRadius(0.5f)
	, m_Scale(Vec2(1.f, 1.f))
	, m_OverlapCount(0)
	, m_Enabled(true)
{

}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: Component(_Origin)
	, m_Shape(_Origin.m_Shape)
	, m_HalfAngle(_Origin.m_HalfAngle)
	, m_WorldCenter(_Origin.m_WorldCenter)
	, m_WorldDir(_Origin.m_WorldDir)
	, m_WorldRadius(_Origin.m_WorldRadius)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_OverlapCount(0)
	, m_Enabled(_Origin.m_Enabled)
{
	// 원본과 같은 OverlapCount와 DELEGATE를 가리키면 안되기 때문에
	// 복사생성자를 직접 구현
}

CCollider2D::~CCollider2D()
{
}


//========
// Overlap
//========
void CCollider2D::BeginOverlap(Ptr<CCollider2D> _Other)
{
	++m_OverlapCount;

	for (size_t i = 0; i < m_vecBeginDel.size(); ++i)
	{
		(m_vecBeginDel[i].Inst->*m_vecBeginDel[i].MemFunc)(this, _Other.Get());
	}

}

void CCollider2D::Overlap(Ptr<CCollider2D> _Other)
{
	for (size_t i = 0; i < m_vecOverDel.size(); ++i)
	{
		(m_vecOverDel[i].Inst->*m_vecOverDel[i].MemFunc)(this, _Other.Get());
	}
}

void CCollider2D::EndOverlap(Ptr<CCollider2D> _Other)
{
	--m_OverlapCount;

	for (size_t i = 0; i < m_vecEndDel.size(); ++i)
	{
		(m_vecEndDel[i].Inst->*m_vecEndDel[i].MemFunc)(this, _Other.Get());
	}

}


//=================
// Overlap 등록 함수
//=================
void CCollider2D::AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecBeginDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}

void CCollider2D::AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecOverDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}

void CCollider2D::AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecEndDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}

void CCollider2D::FinalTick()
{
	/*********************************************************
	* 현재 구조에서는 Render 컴포넌트는 하나만 가질 수 있다.
	* 하지만 개발 시 충돌체의 크기를 눈으로 보면서 작업해야하기 때문에
	* Render 기능이 필요하다.
	*  -> Debug Render 기능 사용
	*********************************************************/
	
	Matrix matTran = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.f);
	// NOTE(26-04-01):
	// 원형과 사각형 충돌 시, 역행렬이 필요하고 z축이 0이면 역행렬 계산에 실패
	//Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.f);

	// Collider2D의 Matrix를 계산하고 (UI의 Drag를 통해 변경 중)
	// 계산된 Matrix를 Transform의 WorldMatrix와 곱해야
	// 최종적으로 m_matWorld의 크기 변화가 이루어 집니다.
	m_matWorld = matScale * matTran;
	m_matWorld *= Transform()->GetWorldMat();

	Vec4 vDbgColor = (0 < m_OverlapCount) ? Vec4(1.f, 0.f, 0.f, 1.f) : ((m_OverlapCount == 0) ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(0,0,0,0));

	m_WorldCenter = Vec3(m_matWorld._41, m_matWorld._42, m_matWorld._43);

	switch (m_Shape)
	{
	case COLLIDER2D_SHAPE::RECT:
		DrawDebugSquare(m_matWorld, vDbgColor, 0.f);
		break;
	case COLLIDER2D_SHAPE::CIRCLE:
		m_WorldRadius = Vec3(m_matWorld._11, m_matWorld._12, m_matWorld._13).Length() * 0.5f;
		DrawDebugCircle(m_WorldCenter, m_WorldRadius, vDbgColor, 0.f);
		break;
	case COLLIDER2D_SHAPE::SECTOR:
	{
		Vec3 vUp = Vec3(0.f, 1.f, 0.f);
		m_WorldDir = XMVector3TransformNormal(vUp, m_matWorld);
		m_WorldDir.Normalize();
		m_WorldRadius = Vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23).Length() * 0.5f;

		DrawDebugSector(m_WorldCenter, m_WorldDir, m_WorldRadius, vDbgColor, 0.f);
	}
	break;
	case COLLIDER2D_SHAPE::LARGE_BASE_CONE:
	{
		m_HalfAngle = 20.f * XM_PI / 180.f;
		Vec3 vUp = Vec3(0.f, 1.f, 0.f);
		m_WorldDir = XMVector3TransformNormal(vUp, m_matWorld);
		m_WorldDir.Normalize();
		m_WorldRadius = Vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23).Length() * 0.5f;

		DrawDebugLargeBaseCone(m_WorldCenter, m_WorldDir, m_WorldRadius, vDbgColor, 0.f);
	}
	break;
	}
}

// 행렬과 충돌카운트는 런타임 중에 결정되므로 파일 저장 X
void CCollider2D::SaveToLevelFile(FILE* _File)
{
	//fwrite(&m_Enabled, sizeof(bool), 1, _File);
	//fwrite(&m_HalfAngle, sizeof(float), 1, _File);
	fwrite(&m_Offset, sizeof(Vec2), 1, _File);
	fwrite(&m_Scale, sizeof(Vec2), 1, _File);
	fwrite(&m_Shape, sizeof(COLLIDER2D_SHAPE), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	//fread(&m_Enabled, sizeof(bool), 1, _File);
	//fread(&m_HalfAngle, sizeof(float), 1, _File);
	fread(&m_Offset, sizeof(Vec2), 1, _File);
	fread(&m_Scale, sizeof(Vec2), 1, _File);
	fread(&m_Shape, sizeof(COLLIDER2D_SHAPE), 1, _File);
}
