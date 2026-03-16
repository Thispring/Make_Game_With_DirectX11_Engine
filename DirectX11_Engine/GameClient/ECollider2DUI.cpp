#include "pch.h"
#include "ECollider2DUI.h"
#pragma warning(disable : 4996)
ECollider2DUI::ECollider2DUI()
	: EComponentUI(COMPONENT_TYPE::COLLIDER2D, "Collider2D")
{
}

ECollider2DUI::~ECollider2DUI()
{
}

void ECollider2DUI::Tick_UI()
{
	OutputTitle("Collider2DUI");
	//ImGui::Text("Collider2DUI");
	if (ImGui::CollapsingHeader("Collider2D", ImGuiTreeNodeFlags_DefaultOpen))
	{
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
