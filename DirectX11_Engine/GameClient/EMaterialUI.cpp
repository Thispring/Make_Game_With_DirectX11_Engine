#include "pch.h"
#include "EMaterialUI.h"

#include "AssetMgr.h"
#include "PathMgr.h"
#include "EditorMgr.h"

#include "ListUI.h"

EMaterialUI::EMaterialUI()
	: EAssetUI(ASSET_TYPE::MATERIAL)
{
}

EMaterialUI::~EMaterialUI()
{
}

void EMaterialUI::Tick_UI()
{
	OutputTitle();

	Ptr<AMaterial> pMtrl = (AMaterial*)GetTargetAsset().Get();

	string key = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##MtrlName", key.data(), key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	
	//=======
	// Shader
	//=======
	ImGui::Text("Shader");
	ImGui::SameLine(120);

	Ptr<AGraphicShader> pShader = pMtrl->GetShader();

	wstring ShaderKey = L"None";
	if (pShader != nullptr)
		ShaderKey = pShader->GetKey();

	ImGui::InputText("##ShaderName", string(ShaderKey.begin(), ShaderKey.end()).data(), ShaderKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 특정 위젯에서 드래그가 발생했고
	if (ImGui::BeginDragDropTarget())
	{
		// 해당 위젯 위에 마우스가 호버링 중이라면
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
		if (PayLoad)
		{
			// PayLoad한 Asset 데이터를 세팅
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::GRAPHICSHADER == pAsset->GetType())
				pMtrl->SetShader((AGraphicShader*)pAsset.Get());
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Shader List");

		vector<wstring> vecShaderNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHICSHADER, vecShaderNames);
		pUI->AddString(vecShaderNames);
		pUI->AddDelegate(this, (DELEGATE_1)&EMaterialUI::SelectShader);
		pUI->SetActive(true);
	}

	// Render Domain
	// Domain
	RENDER_DOMAIN Domain = pMtrl->GetDomain();
	ImGui::Text("Domain Render Setting");
	const char* items[] = { "DOMAIN_OPAQUE", "DOMAIN_MASKED", "DOMAIN_TRANSPARENT", "DOMAIN_POSTPROCESS", "DOMAIN_DEBUG", "DOMAIN_NONE" };
	// Simplified one-liner Combo() using an array of const char*
	// This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
	int item_current_3 = (int)Domain; // If the selection isn't within 0..count, Combo won't display a preview
	if (ImGui::Combo("##Domain Render Setting", &item_current_3, items, IM_COUNTOF(items)))
	{
		// StringToDomain() 호출
		pMtrl->SetDomain(StringToDomain(items[item_current_3]));
	}
	for (int i = 0; i < 5; ++i) ImGui::Spacing();


	// Shader Param
	ShaderParameter();

	// Save Button
	// 버튼을 누르면 해당 경로에 파일 생성
	if (ImGui::Button("Save##MtrlSaveBtn"))
	{
		// 생성된 파일경로가 파일의 위치이자, Key 값으로 사용
		wstring FilePath = CONTENT_PATH + pMtrl->GetKey();
		pMtrl->Save(FilePath);
		ShowSaveMessage(pMtrl->GetKey());
	}
}

void EMaterialUI::SelectShader(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AGraphicShader> pShader = FIND(AGraphicShader, key);

	((AMaterial*)GetTargetAsset().Get())->SetShader(pShader);
}

void EMaterialUI::ShaderParameter()
{
	// Material의 ShaderParameter 정보를 표시하고
	// 변경된 정보를 Setting 하는 함수

	Ptr<AMaterial> pMtrl = (AMaterial*)GetTargetAsset().Get();
	// Shader 설정이 안된 시점이라면 return하여 UI에 표시 X
	if (pMtrl->GetShader() == nullptr)
		return;

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text("Shader Parameter");
	ImGui::Separator();

	const vector<ShaderParam>& vecParam = pMtrl->GetShader()->GetShaderParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SHADER_PARAM::INT:
			break;
		case SHADER_PARAM::FLOAT:
			break;
		case SHADER_PARAM::VEC2:
			break;
		case SHADER_PARAM::VEC4:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine();

			SCALAR_PARAM Param = (SCALAR_PARAM)((UINT)SCALAR_PARAM::VEC4_0 + vecParam[i].Index);
			Vec4& Data = pMtrl->GetScalar<Vec4>(Param);
			ImGui::InputFloat4("##InputFloat4", Data);
		}
		break;
		case SHADER_PARAM::MAT:
			break;
		case SHADER_PARAM::TEX:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());


			Ptr<ATexture> pTex = pMtrl->GetTexture((TEX_PARAM)vecParam[i].Index);
			ImTextureRef SRV = nullptr;

			if (nullptr != pTex)
			{
				SRV = pTex->GetSRV().Get();
			}

			// 이미지 샘플
			ImGui::ImageWithBg(SRV, ImVec2(100, 100)
				, Vec2(0.f, 0.f), Vec2(1.f, 1.f)
				, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;

					if (ASSET_TYPE::TEXTURE == pAsset->GetType())
					{
						pMtrl->SetTexture((TEX_PARAM)vecParam[i].Index, (ATexture*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}
		}
		break;
		}
	}
}
