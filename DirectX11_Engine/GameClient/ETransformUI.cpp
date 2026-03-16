#include "pch.h"
#include "ETransformUI.h"
#include "GameObject.h"

ETransformUI::ETransformUI()
	: EComponentUI(COMPONENT_TYPE::TRANSFORM, "Transform")
{
	// UI이름은 nullptr이 되면 안됨
	// 생성자에서 UI이름을 설정
	//SetUIName("TransformUI");
}

ETransformUI::~ETransformUI()
{
}

void ETransformUI::Tick_UI()
{
	// CollapsingHeader 함수의 파라미터로 enum값 ImGuiTreeNodeFlags_ 를 설정하여,
	// 캡슐안에 들어있는 UI가 어떻게 표시될지 설정할 수 있습니다.
	// ImGuiTreeNodeFlags_DefaultOpen => 기본으로 열기 상태, 이렇게 해야지 스크롤바가 안생겨서 해당 enum으로 설정
	OutputTitle("TransformUI");
	//ImGui::Text("TransformUI");
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
		Vec3 vRot = GetTarget()->Transform()->GetRelativeRot();
		Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();

		ImGui::Text("Position");
		ImGui::SameLine(150);
		if (ImGui::DragFloat3("##POSITION", vPos))
			GetTarget()->Transform()->SetRelativePos(vPos);

		// 회전의 경우, 동작에서는 라디안 단위를 사용하지만,
		// 보여줄때는 쉽게 보여주기 위해 60분법으로 변경
		ImGui::Text("Rotation");
		ImGui::SameLine(150);
		Vec3 vDegree = vRot * 180 / XM_PI;
		if (ImGui::DragFloat3("##ROTATION", vDegree))
		{
			vRot = vDegree * (XM_PI / 180.f);
			GetTarget()->Transform()->SetRelativeRot(vRot);
		}

		ImGui::Text("Scale");
		ImGui::SameLine(150);
		if (ImGui::DragFloat3("##SCALE", vScale))
			GetTarget()->Transform()->SetRelativeScale(vScale);

		bool Independent = GetTarget()->Transform()->IsIndependentScale();

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Independent Scale");
		ImGui::SameLine();
		if (ImGui::Checkbox("##Independent", &Independent))
		{
			GetTarget()->Transform()->SetIndependentScale(Independent);
		}
	}
}
