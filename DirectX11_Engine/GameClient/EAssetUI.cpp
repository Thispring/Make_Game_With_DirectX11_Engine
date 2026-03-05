#include "pch.h"
#include "EAssetUI.h"

EAssetUI::EAssetUI(ASSET_TYPE _Type)
	: EditorUI(ToString(_Type))
	, m_AssetType(_Type)
{
	SetActive(false);
	SetSeparator(false);
}

EAssetUI::~EAssetUI()
{
}

void EAssetUI::OutputTitle()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::Button(ToString(m_AssetType));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}
