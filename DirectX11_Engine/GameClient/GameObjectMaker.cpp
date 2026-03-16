#include "pch.h"
#include "GameObjectMaker.h"

#include "GameObject.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "EditorMgr.h"

#include "Source/ScriptMgr.h"

GameObjectMaker::GameObjectMaker()
	: EditorUI("GameObjectMaker")
	, m_pObject(nullptr)
{
	CreateChildUI();
	m_pObject = new GameObject;
	m_pObject->AddComponent(new CTransform);	// Transform은 기본 생성
}

GameObjectMaker::~GameObjectMaker()
{
}


void GameObjectMaker::SettingClear()
{
}

void GameObjectMaker::Tick_UI()
{
	OutputTitle("Make Game Object", Vec4(0.5f, 0.5f, 0.5f, 0.5f));
	SPACING_UI(5);

	#pragma region Level Setting
	ImGui::Text("Level Setting");
	// wstring -> string 변환
	string levelName = string(m_LevelName.begin(), m_LevelName.end());

	ImGui::SameLine(150);
	if (ImGui::InputText("##LEVELNAME", &levelName))
	{
		wstring wlevelName = wstring(levelName.begin(), levelName.end());
		SetLevelName(wlevelName);
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

			// DragDrop으로 가져온 경우, 파일경로와 확장자, _%d을 제거
			wstring assetName = pAsset->GetKey();

			// 가져온 Texture의 Key 문자열을 세팅
			SetLevelName(assetName);
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Please specify the name of the 'Level'\nto which the GameObject will be added.");
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion	

	#pragma region Name Setting
	ImGui::Text("GameObject name to save");
	// wstring -> string 변환
	string objName = string(m_ObjectName.begin(), m_ObjectName.end());
	if (ImGui::InputText("##GAMEOBJECTNAMETOSAVE", &objName))
	{
		wstring wobjName = wstring(objName.begin(), objName.end());
		SetObjectName(wobjName);
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Please enter the name you want to save.");
	SPACING_UI(5);
	ImGui::Separator();		// 수평 구분선
	#pragma endregion	

	#pragma region Layer Index
	SPACING_UI(5);
	ImGui::Text("Layer Index Setting");
	ImGui::SameLine(150);
	if (ImGui::DragInt("##LAYERIDX", &m_LayerIdx, 1.f, 0, 32))
	{
		SetLayerIdx(m_LayerIdx);
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Enter the Layer index for the GameObject.\n");
	// Level의 현재 설정된 Layer 번호 List를 여기에 출력 
	SPACING_UI(5);
	ImGui::Separator();
	#pragma endregion

	#pragma region Component Setting
	// Component
	ImGui::Text("Component Setting");
	SPACING_UI(5);
	const char* componentNames[] = { "CAMERA", "COLLIDER2D", "LIGHT2D",
			"MESHRENDER", "SPRITE_RENDER", "BILLBOARD_RENDER", "FLIPBOOK_RENDER", "TILE_RENDER", };

	static int currentNum = 0; // If the selection isn't within 0..count, Combo won't display a preview
	if (ImGui::Combo("##Component Setting", &currentNum, componentNames, IM_COUNTOF(componentNames)))
	{
		// 1. ImGui Combo로 Component 문자열을 선택했을때, enum class COMPONENET_TYPE으로 변경하는 함수 호출
		SetComType(StringToComponent(componentNames[currentNum]));
	}
	SPACING_UI(5);

	ImGui::Dummy(ImVec2(0.0f, 0.0f));
	ImGui::SameLine(10.f);
	// 2-1. COMPONENET_TYPE을 받아, 조건에 알맞은 Ptr<Component>를 반환하는 함수 호출
	// 버튼으로 활성화, GameObjectMaker UI에도 Component 정보 표시
	if (ImGui::Button("Add\nComponent", ImVec2(100.f, 100.f)))
	{
		SetComAdd();
	}
	ImGui::SameLine(160.f);
	// 2-2. 등록한 Component를 지우고 싶다면 해당 버튼을 눌러
	// Component 해제 함수 호출
	if (ImGui::Button("Delete\nComponent", ImVec2(100.f, 100.f)))
	{
		m_pObject->ReleaseComponent(GetComType());
	}

	// 3. 반환된 Ptr<Component>를 AddComponent의 매개변수로 전달
	if (IsComAdd() == true)
	{
		// 이미 보유하고 있는 Component라면 리턴
		if (m_pObject->GetComponent(GetComType()) != nullptr)
			return;

		// 이미 RenderComponent가 있다면 리턴
		if (m_pObject->GetRenderCom().Get() != nullptr)
			return;

		// m_Com에 ComType에 맞는 객체를 만들어 전달
		m_pObject->AddComponent(CreateComponent(GetComType()));
	}
	ImGui::Separator();

	// Component의 UI 표시
	SetTargetObject(m_pObject);
	#pragma endregion

	#pragma region Script Setting
	/*************************************************************************************
	* ScriptMgr을 이용해, 현재 보유하고 있는 콘텐츠로 제작한 Script 클래스 네임을 문자열로 받아와,
	* ImGui Combo의 파라미터로 전달합니다.
	* 
	* 같은 이름의 Script를 두개이상 가질 수 없게 예외처리를 구현합니다.
	*************************************************************************************/

	// 이전 프레임 vector 초기화
	m_vecScriptName.clear();

	// ScriptMgr 싱글톤 방식이 아니므로, 지역에 임시 객체 생성
	ScriptMgr scriptMgr;

	// 등록된 SCRIPT_TYPE의 END까지 enum에 해당하는 문자열을
	// m_vecScriptName에 등록
	for (UINT i = 0; i < static_cast<UINT>(SCRIPT_TYPE_END); ++i)
	{
		m_vecScriptName.push_back(scriptMgr.GetScriptName((SCRIPT_TYPE)i));
	}

	
	// Combo를 이용해 추가하려는 콘텐츠 Script 설정
	ImGui::Text("Content Script List");
	SPACING_UI(5);
	static int curNameNum = -1;
	// Combo에 전달하는 항목 수는 벡터의 크기여야 합니다.
	if (ImGui::Combo("##Content Script Names", &curNameNum, m_vecScriptName.data(), static_cast<int>(m_vecScriptName.size())))
	{
		// curNameNum 인덱스에 해당하는 Script 이름 설정
		SetContnentScriptName(m_vecScriptName[curNameNum]);
	}
	SPACING_UI(5);

	ImGui::Dummy(ImVec2(0.0f, 0.0f));
	ImGui::SameLine(10.f);
	// Content Script를 추가하는 로직 조건을 활성화
	// SetContentScriptAdd로 멤버 변수가 true가 되면
	// CScript * ScriptMgr::GetScript(const wstring& _strScriptName) 함수 호출
	if (ImGui::Button("Add\nContent Script", ImVec2(100.f, 100.f)))
	{
		if (m_ContnentScriptName != nullptr)
			SetContentScriptAdd();
	}
	ImGui::SameLine(160.f);

	// 등록한  Content Script 제거 버튼
	if (ImGui::Button("Delete\nContent Script", ImVec2(100.f, 100.f)))
	{
		// SCRIPT_TYPE을 전달
		if (m_ContnentScriptName != nullptr)
			m_pObject->ReleaseContentScript(scriptMgr.GetScriptType(m_ContnentScriptName));
	}

	// CScript * ScriptMgr::GetScript(const wstring& _strScriptName) 함수 호출
	if (IsContentScriptAdd() == true)
	{
		/*************************************************************************************
		* 이미 보유하고 있는 Script라면 리턴
		* 지금 등록하려는 Script가 등록하려는 GameObject의
		* Script vector에 등록되어 있는지를, m_ContnentScriptName에 맞는 enum을 가져와 값을 비교
		*************************************************************************************/
		
		// 현재 추가하려는 Type
		SCRIPT_TYPE addType = scriptMgr.GetScriptType(m_ContnentScriptName);

		// vector 순회했을때, 이미 보유중인 타입이면 return
		vector<Ptr<CScript>> vecScript = m_pObject->GetScripts();
		for (UINT i = 0; i < vecScript.size(); ++i)
		{
			// 보유중인 타입만 검사, m_pObject에 이전에 등록해서 size가 증가했어도
			// 현재 등록하려는 타입이 있는지를 검사
			if (vecScript[i] == nullptr)
				continue;

			if (addType == vecScript[i]->GetScriptType())
				return;
		}

		// 위 조건에 걸리지 않았으면 해당 콘텐츠 Script 추가
		if (m_ContnentScriptName != nullptr)
			m_pObject->AddComponent(scriptMgr.GetScript(m_ContnentScriptName));
	}
	SPACING_UI(5);
	ImGui::Separator();

	#pragma endregion

	#pragma region Content Script File Make
	// 사용자에게 원하는 문자열을 입력받아, 해당 이름으로 작업 디렉터리 Source 파일 경로에 
	// 입력받은 문자열.h, .cpp 파일을 생성 및 저장합니다.
	ImGui::Text("New Add Content Script");
	// wstring -> string 변환
	string addScriptName = string(m_AddScriptName.begin(), m_AddScriptName.end());
	if (ImGui::InputText("##CONTENTSCRIPTNAMETOSAVE", &addScriptName))
	{
		wstring waddScriptName = wstring(addScriptName.begin(), addScriptName.end());
		SetAddScriptName(waddScriptName);
	}
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
		"Please enter the name you want to save.");
	SPACING_UI(5);

	// Button으로 생성
	if (ImGui::Button("New Add\nContent Script", ImVec2(100.f, 50.f)))
	{
		SetScriptAdd();
	}
	SPACING_UI(5);

	if (IsScriptAdd())
	{
		scriptMgr.GenerateScriptFiles(m_AddScriptName);
	}
	SPACING_UI(5);


	ImGui::Separator();

	#pragma endregion	


	#pragma region ObjectSaveBtn
	// 저장하기 전에 설정이 올바른지 확인합니다.
	// ex) Layer Index가 지정된 인덱스 범위 밖에 있다면 크래시
	// GameObject 생성은 LevelMgr에서 담당

	if (ImGui::Button("Make Game Object", ImVec2(100.f, 200.f)))
	{
		SetObjectMake();
	}
	
	if (IsObjectMake())
	{
		// LevelMgr로 넘기기 전에 이름 설정
		m_pObject->SetName(m_ObjectName);
		Ptr<ALevel> pLevel = AssetMgr::GetInst()->FIND(ALevel, m_LevelName);

		LevelMgr::GetInst()->AddNewObject(m_pObject, pLevel, m_LayerIdx);
	}

	#pragma endregion

	SPACING_UI(5);
}

void GameObjectMaker::SetTargetObject(Ptr<GameObject> _Object)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		// 자식 UI 클래스의 멤버에도 오브젝트 정보를 등록
		m_arrComUI[i]->SetTarget(_Object);
	}

	// TargetObject가 Script를 얼마나 가지고 있는지 검사, 등록
	if (_Object != nullptr)
	{
		// 오브젝트의 Script에 대응하는 ScriptUI를 활성/비활성화
		const vector<Ptr<CScript>>& vecScripts = _Object->GetScripts();

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

}
