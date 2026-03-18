#include "pch.h"
#include "MaterialMaker.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "imguiFunc.h"

MaterialMaker::MaterialMaker()
	: EditorUI("MaterialMaker")
	, m_MtrlName {}
	, m_ShaderName {}
	, m_TextureName {}
	, m_RenderDomain(RENDER_DOMAIN::DOMAIN_NONE)
{
}

MaterialMaker::~MaterialMaker()
{
}

void MaterialMaker::ClearSetting()
{
	m_MtrlName = {};
	m_ShaderName = {};
	m_TextureName = {};
	m_RenderDomain = RENDER_DOMAIN::DOMAIN_NONE;
}

void MaterialMaker::Tick_UI()
{
	// 크기 조절을 위해 OutputTitle 함수 사용 X
	Vec4 vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vColor);
	ImGui::Button("Making Material", Vec2(150.f, 50.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine(300.f);
	#pragma region MaterialSaveBtn
	if (ImGuiFunc::ColoredButton("SaveMaterial##MtrlSaveBtn", ColorConvertIntToVec4(20, 166, 34), ImVec2(150.f, 50.f)))
	{
		// 버튼을 누르면 팝업 상태를 'Open'으로 설정
		ImGui::OpenPopup("MtrlSave?");
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// 모달 창을 이용해 저장하기 전 메시지를 띄우기
	if (ImGui::BeginPopupModal("MtrlSave?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Please check if the values are correct!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 및 초기화
			// AssetMgr 호출
			AssetMgr::GetInst()->CreateEngineMaterial(m_MtrlName, m_TextureName, m_ShaderName, m_RenderDomain);
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

	#pragma region ShaderName
	OutputTitle("Shader Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string shaderName = string(m_ShaderName.begin(), m_ShaderName.end());
	if (ImGui::InputTextWithHint("##SHADERNAME", "Example: std2DShader", &shaderName))
	{
		wstring wSName = wstring(shaderName.begin(), shaderName.end());
		SetShaderName(wSName);
	}

	// InputText에서 혹시 Drop 받은 Payload가 있는지 체크
	// Drop 체크는, 특정 위젯에서 드래그가 발생 && 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			// 가져온 Texture의 Key 문자열을 세팅
			SetShaderName(pAsset->GetKey());
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Drag and drop an image from the ContentUI\nGraphicShader list into the box above.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Texture Name
	OutputTitle("Texture Name", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string tName = string(m_TextureName.begin(), m_TextureName.end());
	if (ImGui::InputTextWithHint("##MM_TEXTURENAME", "Example: cubeImg", &tName))
	{
		wstring wtName = wstring(tName.begin(), tName.end());
		SetMtrlName(wtName);
	}
	
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");	// Content UI에서만 받도록 Key 조건 설정
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			// 가져온 Texture의 Key 문자열을 세팅
			SetTextureName(pAsset->GetKey());
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Drag and drop an image from the ContentUI\nTexture list into the box above.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Material name to save
	OutputTitle("Material name to save", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	// wstring -> string 변환
	string mName = string(m_MtrlName.begin(), m_MtrlName.end());
	if (ImGui::InputTextWithHint("##MTRLNAMETOSAVE", "Name your Material", &mName))
	{
		wstring wmName = wstring(mName.begin(), mName.end());
		SetMtrlName(wmName);
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
		"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Domain
	OutputTitle("Domain Render Setting", ColorConvertIntToVec4(4.f, 135.f, 35.f));
	const char* items[] = { "DOMAIN_OPAQUE", "DOMAIN_MASKED", "DOMAIN_TRANSPARENT", "DOMAIN_POSTPROCESS", "DOMAIN_DEBUG", "DOMAIN_NONE" };
	// Simplified one-liner Combo() using an array of const char*
	// This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
	static int item_current_3 = 0; // If the selection isn't within 0..count, Combo won't display a preview
	if (ImGui::Combo("##Domain Render Setting", &item_current_3, items, IM_COUNTOF(items)))
	{
		// StringToDomain() 호출
		SetRenderDomain(StringToDomain(items[item_current_3]));
	}
	ImGui::Spacing();
	IMGUI_REQUIRED()
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

}