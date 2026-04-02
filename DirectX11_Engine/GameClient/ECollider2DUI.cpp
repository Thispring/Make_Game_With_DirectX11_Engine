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

COLLIDER2D_SHAPE ECollider2DUI::StringToShape(const char* _Str)
{
	if (_Str == "RECT")
		return COLLIDER2D_SHAPE::RECT;
	else if (_Str == "CIRCLE")
		return COLLIDER2D_SHAPE::CIRCLE;
	else if (_Str == "SECTOR")
		return COLLIDER2D_SHAPE::SECTOR;
	else if (_Str == "LARGE_BASE_CONE")
		return COLLIDER2D_SHAPE::LARGE_BASE_CONE;
}

const char* ECollider2DUI::ShapeToString(COLLIDER2D_SHAPE _Shape)
{
	switch (_Shape)
	{
	case COLLIDER2D_SHAPE::RECT: return "RECT";
		break;
	case COLLIDER2D_SHAPE::CIRCLE: return "CIRCLE";
		break;
	case COLLIDER2D_SHAPE::SECTOR: return "SECTOR";
		break;
	case COLLIDER2D_SHAPE::LARGE_BASE_CONE: return "LARGE_BASE_CONE";
		break;
	}
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

		// 충돌 되었는지 카운트
		ImGui::Text("OverlapCount");
		ImGui::SameLine(150);
		int overlap = GetTarget()->Collider2D()->GetOverlapCount();
		ImGui::DragInt("##OVERLAPCOUNT", &overlap);


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


		bool isEnabled = GetTarget()->Collider2D()->IsEnabled();
		ImGui::Text("Enabled");
		ImGui::SameLine(150);
		if (ImGui::Checkbox("##ENABLED", &isEnabled))
		{
			GetTarget()->Collider2D()->SetEnabled(isEnabled);
		}


		ImGui::Text("Shape");
		ImGui::SameLine(150);
		COLLIDER2D_SHAPE shape = GetTarget()->Collider2D()->GetShape();
		const char* shapeName = ShapeToString(shape);

		const char* items[] = { "RECT", "CIRCLE", "SECTOR", "LARGE_BASE_CONE" };
		static int item_current = 0; // If the selection isn't within 0..count, Combo won't display a preview
		for (int i = 0; i < 4; ++i)
		{
			if (shapeName == items[i])
			{
				item_current = i;
				break;
			}
		}
		// Simplified one-liner Combo() using an array of const char*
		// This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
		if (ImGui::Combo("##SHAPE", &item_current, items, IM_COUNTOF(items)))
		{
			GetTarget()->Collider2D()->SetShape(StringToShape(items[item_current]));
		}


		float halfAngle = GetTarget()->Collider2D()->GetHalfAngle();
		ImGui::Text("HalfAngle");
		ImGui::SameLine(150);
		if (ImGui::DragFloat("##HALFANGLE", &halfAngle))
		{
			GetTarget()->Collider2D()->SetHalfAngle(halfAngle);
		}
	}
	
}
