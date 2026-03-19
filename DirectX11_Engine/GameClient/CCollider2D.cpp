#include "pch.h"
#include "CCollider2D.h"
#include "RenderMgr.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Scale(Vec2(1.f, 1.f))
	, m_OverlapCount(0)
{

}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: Component(_Origin)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_OverlapCount(0)
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
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 0.f);

	// Collider2D의 Matrix를 계산하고 (UI의 Drag를 통해 변경 중)
	// 계산된 Matrix를 Transform의 WorldMatrix와 곱해야
	// 최종적으로 m_matWorld의 크기 변화가 이루어 집니다.
	m_matWorld = matScale * matTran;
	m_matWorld *= Transform()->GetWorldMat();

	if (0 < m_OverlapCount)
		DrawDebugSquare(m_matWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f);
	else if (m_OverlapCount == 0)
		DrawDebugSquare(m_matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	else
		assert(nullptr);
}

// 행렬과 충돌카운트는 런타임 중에 결정되므로 파일 저장 X
void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Offset, sizeof(Vec2), 1, _File);
	fwrite(&m_Scale, sizeof(Vec2), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Offset, sizeof(Vec2), 1, _File);
	fread(&m_Scale, sizeof(Vec2), 1, _File);
}
