#include "pch.h"
#include "EAssetUI.h"

EAssetUI::EAssetUI(ASSET_TYPE _Type)
	: EditorUI(ToString(_Type))
	, m_AssetType(_Type)
	, m_IsShowSaveMessage(false)
	, m_SaveMessage{}
{
	SetActive(false);
	SetSeparator(false);
}

EAssetUI::~EAssetUI()
{
}

void EAssetUI::OutputTitle()
{
	UpdateMessage();
	if (m_IsShowSaveMessage)
	{
		// ImGui 예시
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "%ls", m_SaveMessage.c_str());
	}

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

void EAssetUI::ShowSaveMessage(const wstring& _Msg)
{
	m_SaveMessage = _Msg + L" Save Complete";
	m_MessageStartTime = std::chrono::steady_clock::now();
	m_IsShowSaveMessage = true;
}

void EAssetUI::UpdateMessage()
{
	if (m_IsShowSaveMessage)
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_MessageStartTime).count();
		if (elapsed >= 2)
		{
			m_IsShowSaveMessage = false;
			m_SaveMessage.clear();
		}
	}
}
