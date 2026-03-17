#include "pch.h"
#include "ECollider2DUI.h"
#pragma warning(disable : 4996)
ECollider2DUI::ECollider2DUI()
	: EComponentUI(COMPONENT_TYPE::COLLIDER2D, "Collider2D")
	, m_IsDelete(false)
{
}

ECollider2DUI::~ECollider2DUI()
{
}

void ECollider2DUI::Tick_UI()
{
	OutputTitle("Collider2DUI");

	// 컴포넌트 제거 버튼
	DeleteComponent(COMPONENT_TYPE::COLLIDER2D);

	if (ImGui::CollapsingHeader("Collider2D", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Collider2D 정보가 아직 없다면 이 부분은 return 시키기
		if (GetTarget()->Collider2D() == nullptr)
			return;

		// Target Object의 정보를 얻어오기
		Vec2 vScale = GetTarget()->Collider2D()->GetScale();
		Vec2 vOffset = GetTarget()->Collider2D()->GetOffset();

		// Matrix의 경우 ImGui에서 충돌할 Layer 대상을 선택하도록 구현하기

		ImGui::Text("Scale");
		ImGui::SameLine(150);
		if (ImGui::DragFloat2("##SCALE", vScale))
		{
			GetTarget()->Collider2D()->SetScale(vScale);
		}

		ImGui::Text("Offset");
		ImGui::SameLine(150);
		if (ImGui::DragFloat2("##OFFSET", vOffset))
		{
			GetTarget()->Collider2D()->SetOffset(vOffset);
		}
	}
	
}
