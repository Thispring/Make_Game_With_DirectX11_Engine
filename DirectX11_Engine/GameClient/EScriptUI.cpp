#include "pch.h"
#include "EScriptUI.h"

#include "TreeUI.h"
#include "imguiFunc.h"
#include "LevelMgr.h"
#include <Source/ScriptMgr.h>

EScriptUI::EScriptUI()
	: EComponentUI(COMPONENT_TYPE::SCRIPT, "ScriptUI")
	, m_ItemHeight(0)
{
	/******************************************************************************************
	* Inspector 가 표기하려는 GameObject 가 여러개의 Script 를 가지고 있을 수 있기 때문에
	* 각 Script 에 대응하는 ScriptUI 도 여러개가 될 수 있다.
	* 따라서 ScriptUI 끼리 Inspector 의 자식으로서 서로의 이름이 겹치지 않도록 추가로 Key를 작성한다
	******************************************************************************************/
	int idx = GetID();
	char szNum[50] = {};
	_itoa_s(idx, szNum, 50, 10);

	SetUIKey(szNum);
}

EScriptUI::~EScriptUI()
{
}

void EScriptUI::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	if (nullptr == m_TargetScript)
		SetActive(false);
	else
		SetActive(true);
}

void EScriptUI::Tick_UI()
{
	m_ItemHeight = 0;

	// 스크립트 이름 출력
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.3f, 1.f));

	wstring WScriptName = ScriptMgr::GetScriptName(m_TargetScript.Get());
	string ScriptName = string(WScriptName.begin(), WScriptName.end());

	ImGui::Button(ScriptName.c_str());
	AddItemHeight();

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// 보유 스크립트 컴포넌트 제거 버튼
	DeleteScript((SCRIPT_TYPE)m_TargetScript->GetScriptType());

	// Script 파라미터
	const vector<tScriptParam>& vecParam = m_TargetScript->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		// 같은 타입의 파라미터를 여러개 가질 수 있기에
		// ID에 i값을 문자열로 바꿔 저장
		char ID[255] = {};
		sprintf_s(ID, 255, "%d", i);

		switch (vecParam[i].Param)
		{
		case SCRIPT_PARAM::INT:
			break;
		case SCRIPT_PARAM::FLOAT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Float";
			Key += ID;

			if (vecParam[i].IsInput)
				ImGui::InputFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);
			else
				ImGui::DragFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);
			
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::VEC2:
			break;
		case SCRIPT_PARAM::VEC4:
			break;
		case SCRIPT_PARAM::MATRIX:
			break;
		case SCRIPT_PARAM::TEXTURE:
			break;
		case SCRIPT_PARAM::MATERIAL:
			break;
		case SCRIPT_PARAM::STRING:
			break;
		default:
			break;
		}
	}

	SetSizeAsChild(Vec2(0.f, (float)m_ItemHeight));
}

void EScriptUI::AddItemHeight()
{
	ImVec2 vSize = ImGui::GetItemRectSize();
	m_ItemHeight += vSize.y + 5.f;
}

bool EScriptUI::DeleteScript(SCRIPT_TYPE _Type)
{
	// Tick_UI에서 아래 파라미터를 DeleteScript에 전달합니다.
	//m_TargetScript->GetScriptType();

	// Inspector 에서만 Delete UI를 표시하도록 부모 UI를 검사
	Ptr<EditorUI> pParent = GetParentUI();
	if (pParent == nullptr)
		return false;

	// 부모 UI의 이름이 "Inspector"일 때만 삭제 UI 노출
	if (pParent->GetUIName() != "Inspector")
		return false;

	ImGui::SameLine(250.f);

	if (ImGuiFunc::ColoredButton("Delete\nThis Script",
		ColorConvertIntToVec4(125, 23, 20), ImVec2(120.f, 40.f)))
	{
		ImGui::OpenPopup("DeleteThisScript?");
	}

	if (ImGui::BeginPopupModal("DeleteThisScript?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want delete this script?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// m_TargetScript->GetOwner()로 접근해야, 실제 가리키고 있는 GameObject 주소를 얻어옴
			// GetTarget()만을 사용하면 nullptr을 반환
			Ptr<GameObject> pObject = m_TargetScript->GetOwner();
			pObject->ReleaseContentScript(_Type);

			// 현재 Level에 변경점을 알림
			// LevelMgr의 ChangeLevel는 private 함수
			Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
			ChangeLevel(pLevel->GetKey());
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			return true;
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}
	SPACING_UI(5);

	return false;
}
