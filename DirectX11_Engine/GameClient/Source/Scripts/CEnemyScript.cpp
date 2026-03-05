#include "pch.h"
#include "CEnemyScript.h"
#include "GameObject.h"

CEnemyScript::CEnemyScript()
	: CScript(SCRIPT_TYPE::ENEMYSCRIPT)
	, m_HP(10)
	, m_Time(0)
{
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (nullptr != GetOwner()->GetRenderCom() || nullptr != GetOwner()->GetRenderCom()->GetMaterial())
	{
		// 가리키고 있던 원본재질을 복사생해서 자신만의 재질을 만들고, 그것을 반환
		Ptr<AMaterial> pDynamicMtrl = GetOwner()->GetRenderCom()->CreateDynamicMaterial();
		pDynamicMtrl->SetScalar(INT_0, 1);

		//FlipbookRender()->GetMtrl()->SetScalar(INT_0, 1);
	}

	TakeDamage(50);
}

void CEnemyScript::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	//TakeDamage(10);
}

void CEnemyScript::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (nullptr != GetOwner()->GetRenderCom() || nullptr != GetOwner()->GetRenderCom()->GetMaterial())
	{
		// 현재 사용중인 재질을 원본 재질을 가리키게 함
		GetOwner()->GetRenderCom()->GetSharedMaterial();
	}
}

void CEnemyScript::TakeDamage(float _Damage)
{
	if (m_HP <= 0)
		Destroy();
	
	m_HP -= _Damage;
}

void CEnemyScript::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CEnemyScript::BeginOverlap);
	ADD_DYNAMIC_OVERLAP(CEnemyScript::Overlap);
	ADD_DYNAMIC_END_OVERLAP(CEnemyScript::EndOverlap);
}

void CEnemyScript::Tick()
{
	m_Time += 10 * DT;
}
