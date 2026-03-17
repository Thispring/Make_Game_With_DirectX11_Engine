#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"
#include "GameObject.h"

#include "imguiFunc.h"
#include "Source\ScriptMgr.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	CreateChildUI();
	SetTargetObject(nullptr);
}

Inspector::~Inspector()
{
}


void Inspector::Tick_UI()
{
	// m_TargetObject로 받아온 GameObject 정보를 ImGui에 출력
	// GameObject의 정보 출력은 Inspector에서 직접처리하지 않고,
	// Inspector의 자식 UI클래스에서 담당합니다.
	if (m_TargetObject == nullptr)
		return;

	wstring Name = m_TargetObject->GetName();
	string strName = string(Name.begin(), Name.end());

	if (strName.empty())
		strName = "No Name";

	ImGui::Button(strName.c_str());

	// 위 버튼과 같은 라인 끝쪽에 삭제 버튼 추가하기
	ImGui::SameLine(250.f);

	// 색상있는 버튼 UI 생성하는 함수, imguiFunc.cpp에 구현
	if (ImGuiFunc::ColoredButton("Destroy", ImVec4(1.f, 0.2f, 0.2f, 1.f), ImVec2(100.f, 20.f)))
	{
		ImGui::OpenPopup("Destroy_GameObject?");
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Destroy_GameObject?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want destroy this GameObject?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// GameObject Destroy 함수 호출
			m_TargetObject->Destroy();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	SPACING_UI(10);

	#pragma region Add Component 버튼 
	vector<UINT> vecComType = {};

	COMPONENT_TYPE tempType = (COMPONENT_TYPE)0;
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		// GetComponent가 nullptr이라면 보유하지 않은 Component
		if (nullptr != GetTargetObject()->GetComponent((COMPONENT_TYPE)i))
		{
			// COMPONENT_TYPE에 정의된 숫자와
			// Combo로 추가할 Component 요소의 개수가 다르므로
			// 수동으로 알맞은 정수를 추가합니다.
			switch ((COMPONENT_TYPE)i)
			{
			case COMPONENT_TYPE::CAMERA: vecComType.push_back(0);
				break;
			case COMPONENT_TYPE::COLLIDER2D: vecComType.push_back(1);
				break;
			case COMPONENT_TYPE::LIGHT2D: vecComType.push_back(2);
				break;

			// Render Component는 하나만 있어도 전부 비활성화
			case COMPONENT_TYPE::MESHRENDER: 
			case COMPONENT_TYPE::SPRITE_RENDER:
			case COMPONENT_TYPE::BILLBOARD_RENDER:
			case COMPONENT_TYPE::FLIPBOOK_RENDER:
			case COMPONENT_TYPE::TILE_RENDER:
				vecComType.push_back(3);
				vecComType.push_back(4);
				vecComType.push_back(5);
				vecComType.push_back(6);
				vecComType.push_back(7);
				break;
			}
			
		}

	}

	// Component List Combo
	ImGui::Text("Component List");
	const char* componentNames[] = { "CAMERA", "COLLIDER2D", "LIGHT2D",
			"MESHRENDER", "SPRITE_RENDER", "BILLBOARD_RENDER", "FLIPBOOK_RENDER", "TILE_RENDER", };

	static int currentNum = -1; // If the selection isn't within 0..count, Combo won't display a preview
	
	// BeginCombo / Selectable로 직접 그리기
	const char* preview = (currentNum >= 0 && currentNum < IM_COUNTOF(componentNames)) ? componentNames[currentNum] : " ";
	static string comName = {};
	// 컨텐츠 영역의 절반 크기로 설정
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
	if (ImGui::BeginCombo("##Inspector Component Setting", preview))
	{
		for (int i = 0; i < IM_COUNTOF(componentNames); ++i)
		{
			// i가 vecComType에 존재하면 disabled = true
			bool disabled = (find(vecComType.begin(), vecComType.end(), (UINT)i) != vecComType.end());

			// 비활성 조건: 예시로 인덱스 2를 비활성화
			// 실제로는 m_TargetObject나 다른 상태를 검사하여 설정하세요.
			if (disabled)
				ImGui::BeginDisabled();

			// Selectable 클릭 시 currentNum 갱신 (disabled일 경우 선택 동작이 무시됨)
			if (ImGui::Selectable(componentNames[i], currentNum == i) && !disabled)
			{
				currentNum = i;
				comName = componentNames[i];
			}

			if (disabled)
			{
				ImGui::EndDisabled();
			}
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine(250.f);

	// Add Component 버튼 추가
	if (ImGuiFunc::ColoredButton("Add Component", ColorConvertIntToVec4(20, 166, 34), ImVec2(100.f, 20.f)))
	{
		GetTargetObject()->AddComponent(CreateComponent(StringToComponent(comName)));
		// 초기화
		currentNum = -1;
		// 현재 Level에 변경점을 알림
		// LevelMgr의 ChangeLevel는 private 함수
		Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
		ChangeLevel(pLevel->GetKey());
		return;
	}
	
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion


	#pragma region Add Script 버튼
	// Script 목록을 저장할 벡터
	vector<const char*> vecScriptName = {};

	// ScriptMgr 싱글톤 방식이 아니므로, 지역에 임시 객체 생성
	ScriptMgr scriptMgr;

	// 등록된 SCRIPT_TYPE의 END까지 enum에 해당하는 실제 Script의 문자열을 등록
	for (UINT i = 0; i < static_cast<UINT>(SCRIPT_TYPE_END); ++i)
	{
		vecScriptName.push_back(scriptMgr.GetScriptName((SCRIPT_TYPE)i));
	}

	// Target 오브젝트의 스크립트 목록 벡터를 가져옵니다.
	// 벡터의 크기만큼 어떤 스크립트를 보유중인지 Key값을 가져옵니다.
	vector<Ptr<CScript>> pScript = GetTargetObject()->GetScripts();
	vector<UINT> vecScriptType = {};
	
	for (UINT i = 0; i < pScript.size(); ++i)
	{
		vecScriptType.push_back(pScript[i]->GetScriptType());
	}

	ImGui::Text("Content Script List");
	static int currentScriptNum = -1;
	const char* scriptPreview = (currentScriptNum >= 0 && currentScriptNum < vecScriptName.size()) ? vecScriptName[currentScriptNum] : " ";
	static string scriptName = {};
	// 컨텐츠 영역의 절반 크기로 설정
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
	if (ImGui::BeginCombo("##Inspector Script Setting", scriptPreview))
	{
		for (UINT i = 0; i < vecScriptName.size(); ++i)
		{
			bool disabled = (find(vecScriptType.begin(), vecScriptType.end(), (UINT)i) != vecScriptType.end());

			if (disabled)
				ImGui::BeginDisabled();

			// Selectable 클릭 시 currentNum 갱신 (disabled일 경우 선택 동작이 무시됨)
			if (ImGui::Selectable(vecScriptName[i], currentScriptNum == i) && !disabled)
			{
				currentScriptNum = i;
				scriptName = vecScriptName[i];
			}

			if (disabled)
			{
				ImGui::EndDisabled();
			}
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine(250.f);

	// Add Script 버튼 추가
	if (ImGuiFunc::ColoredButton("Add Script", ColorConvertIntToVec4(20, 166, 34), ImVec2(100.f, 20.f)))
	{
		// ScriptMgr에서 아래 함수 호출
		// CScript * ScriptMgr::GetScript(const char* _strScriptName)
		GetTargetObject()->AddComponent(scriptMgr.GetScript(scriptName.c_str()));
		
		// 초기화
		currentScriptNum = -1;
		// 현재 Level에 변경점을 알림
		// LevelMgr의 ChangeLevel는 private 함수
		Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
		ChangeLevel(pLevel->GetKey());
		return;
	}

	SPACING_UI(5);
	ImGui::Separator();



	ImGui::Separator();
	#pragma endregion

}

/****************************************************************************
* Inspector에서 Target되고 있는 UI는 하나만 존재해야 하므로,
* Set함수에서 Object의 Component, Asset 둘 중 하나만 표시하도록 조건을 설정합니다.
****************************************************************************/
void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		// 자식 UI 클래스의 멤버에도 오브젝트 정보를 등록
		m_arrComUI[i]->SetTarget(m_TargetObject);
	}		

	// TargetObject가 Script를 얼마나 가지고 있는지 검사, 등록
	if (m_TargetObject != nullptr)
	{
		// 오브젝트의 Script에 대응하는 ScriptUI를 활성/비활성화
		const vector<Ptr<CScript>>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트가 보유한 Script 개수에 비해서 대응할 ScriptUI의 개수가 모자르면
		// 동적으로 추가합니다.
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			int AddCount = vecScripts.size() - m_vecScriptUI.size();

			for (int i = 0; i < AddCount; ++i)
			{
				EScriptUI* pScriptUI = new EScriptUI;
				pScriptUI->SetSizeAsChild(Vec2(0.f, 150.f));
				AddChildUI(pScriptUI);

				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 오브젝트에서 가져온 Script를 각각의 ScriptUI에 세팅
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (vecScripts.size() <= i)
				m_vecScriptUI[i]->SetScript(nullptr);
			else
				m_vecScriptUI[i]->SetScript(vecScripts[i].Get());
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI를 비활성화
	m_TargetAsset = nullptr;
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
		m_arrAssetUI[i]->SetActive(false);

}

void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
	// ComponentUI 비활성화
	SetTargetObject(nullptr);

	// 입력으로 들어온 Asset UI를 활성화
	m_TargetAsset = _Asset;
	
	// 들어온 정보가 null이 아닐때만 활성화
	if (m_TargetAsset == nullptr)
	{
		for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
			m_arrAssetUI[i]->SetActive(false);
	}
	else
	{
		ASSET_TYPE Type = m_TargetAsset->GetType();
		m_arrAssetUI[(UINT)Type]->SetActive(true);
		m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
	}
}
