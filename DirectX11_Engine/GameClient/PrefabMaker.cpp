#include "pch.h"
#include "PrefabMaker.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "imguiFunc.h"

PrefabMaker::PrefabMaker()
	: EditorUI("PrefabMaker")
	, m_Object{}
{
}

PrefabMaker::~PrefabMaker()
{
}


void PrefabMaker::ClearSetting()
{
}

void PrefabMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making Prefab", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine(300.f);

	#pragma region PrefabSaveBtn
	if (ImGuiFunc::ColoredButton("SavePrefab##PrefabSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("PrefabSave?");
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("PrefabSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 및 초기화
			// AssetMgr 호출
			AssetMgr::GetInst()->CreateEnginePrefab(m_Object);
			ClearSetting();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	SPACING_UI(7);
	ImGui::Separator();
	#pragma endregion	

	#pragma region Setting GameObject
	OutputTitle("Setting GameObject", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string objectName = {};

	if (m_Object != nullptr)
		objectName = string(m_Object->GetName().begin(), m_Object->GetName().end());

	if (ImGui::InputTextWithHint("##PREFABNAME", "Example: GameObject", &objectName, ImGuiInputTextFlags_ReadOnly))
	{

	}

	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Outliner");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<GameObject> pObject = (GameObject*)data;

			SetObject(pObject);
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
			"Drag and drop a GameObject from the Outliner to create a Prefab.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

}