#include "pch.h"
#include "EComponentUI.h"


EComponentUI::EComponentUI(COMPONENT_TYPE _Type, const string& _Name)
	: EditorUI(_Name)
	, m_ComType(_Type)
{
}

EComponentUI::~EComponentUI()
{
}

void EComponentUI::SetTarget(Ptr<GameObject> _Object)
{
	m_Target = _Object;

	if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_ComType))
		SetActive(false);
	else
		SetActive(true);
}

void EComponentUI::OutputTitle(const string& _Title)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::Button(_Title.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}

void EComponentUI::OutputTitle(const string& _Title, ImVec4 _Color)
{
	ImGui::PushID(0);
	// RGB 값을 모두 최대로 설정 (255, 255, 255)
	ImGui::PushStyleColor(ImGuiCol_Button, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _Color);
	ImGui::Button(_Title.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}