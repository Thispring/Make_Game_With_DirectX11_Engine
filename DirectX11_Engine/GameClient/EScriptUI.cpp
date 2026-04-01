#include "pch.h"
#include "EScriptUI.h"

#include "TreeUI.h"
#include "imguiFunc.h"
#include "LevelMgr.h"
#include "EditorMgr.h"
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
	// 비활성화로 요소 출력을 막기보단, Script 멤버가 복사되는지 확인
	//if (m_TargetScript->GetOwner()->GetIsActive() == false)
	//	return;

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

	ImGui::Separator();
	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		// 같은 타입의 파라미터를 여러개 가질 수 있기에
		// ID에 i값을 문자열로 바꿔 저장
		char ID[255] = {};
		sprintf_s(ID, 255, "%d", i);

		switch (vecParam[i].Param)
		{
		case SCRIPT_PARAM::BOOL:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Bool";
			Key += ID;

			ImGui::Checkbox(Key.c_str(), (bool*)vecParam[i].Data);
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::INT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Int";
			Key += ID;

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
			if (vecParam[i].IsInput)
				ImGui::InputInt(Key.c_str(), (int*)vecParam[i].Data, vecParam[i].Step);
			else
				ImGui::DragInt(Key.c_str(), (int*)vecParam[i].Data, vecParam[i].Step);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::FLOAT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Float";
			Key += ID;

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
			if (vecParam[i].IsInput)
			{
				if (ImGui::InputFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step))
				{
					// 값이 변경됨. *pVal 에 이미 새 값이 들어감.
					// 필요하면 여기서 추가 처리 (예: 레벨 변경 플래그 or 변경사항 저장)
					// 
					// NOTE(26-04-01):
					// 아직 변경점을 저장하는 기능이 필요하진않아서
					// 변경점 저장 기능이 필요할때, 여기서 Level Save 호출
					//LevelMgr::GetInst()->GetCurLevel()->SetChanged();
				}
			}
			else
			{
				// NOTE(26-04-01):
				// DragFloat 실제로 드래그하는 옵션이 꺼져있는걸로 확인
				// 추후 드래그로 값 변경하길 원하면 아래 함수 작동을 확인하고 수정
				ImGui::DragFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);
			}
			
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC2:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());

			string Key = "##Vec2";
			Key += ID;

			if (vecParam[i].IsInput)
				ImGui::InputFloat2(Key.c_str(), (float*)vecParam[i].Data);
			else
				ImGui::DragFloat2(Key.c_str(), (float*)vecParam[i].Data);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC4:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Vec4";
			Key += ID;

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
			if (vecParam[i].IsInput)
				ImGui::InputFloat4(Key.c_str(), (float*)vecParam[i].Data);
			else
				ImGui::DragFloat4(Key.c_str(), (float*)vecParam[i].Data);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::MATRIX:
			break;
		case SCRIPT_PARAM::TEXTURE:
			break;
		case SCRIPT_PARAM::MATERIAL:
			break;
		case SCRIPT_PARAM::PREFAB:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Prefab";
			Key += ID;

			Ptr<APrefab> pPrefab = *((Ptr<APrefab>*)vecParam[i].Data);

			string PrefabName = "None";
			if (nullptr != pPrefab)
			{
				PrefabName = string(pPrefab->GetKey().begin(), pPrefab->GetKey().end());
			}

			ImGui::InputText(Key.c_str(), PrefabName.data(), PrefabName.length() + 1, ImGuiInputTextFlags_ReadOnly);
			AddItemHeight();

			// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;

					if (ASSET_TYPE::PREFAB == pAsset->GetType())
					{
						*((Ptr<APrefab>*)vecParam[i].Data) = ((APrefab*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::ENUM_CLASS:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##EnumClass";
			Key += ID;

			// 옵션 목록(wstring)을 가져와서 char* 배열로 변환
			const vector<wstring>& enumOpts = vecParam[i].EnumOptions;
			vector<string> optUtf8;
			vector<const char*> optPtrs;

			if (enumOpts.empty())
			{
				// 옵션이 없다면 빈 Combo를 표시
				const char* items[] = { "" };
				static int item_current = 0;
				ImGui::Combo(Key.c_str(), &item_current, items, IM_ARRAYSIZE(items));
			}
			else
			{
				optUtf8.reserve(enumOpts.size());
				optPtrs.reserve(enumOpts.size());
				for (const auto& w : enumOpts)
				{
					// 간단 ASCII 변환 (프로젝트에서 UTF-8 변환 유틸이 있으면 그걸 사용)
					optUtf8.emplace_back(string(w.begin(), w.end()));
					optPtrs.push_back(optUtf8.back().c_str());
				}

				// 현재 값(포인터가 가리키는 enum)을 인덱스로 변환
				int curIndex = 0;
				// enum의 기본값이 int로 표현 가능하다고 가정
				curIndex = static_cast<int>(*(int*)vecParam[i].Data);

				if (curIndex < 0) curIndex = 0;
				if (curIndex >= (int)optPtrs.size()) curIndex = 0;

				if (ImGui::Combo(Key.c_str(), &curIndex, optPtrs.data(), (int)optPtrs.size()))
				{
					// 변경 발생 시 실제 enum 값 업데이트
					*(int*)vecParam[i].Data = curIndex;

					// 필요하면 레벨 변경 플래그 처리 (예: ChangeLevel 호출 또는 에디터에 notify)
					// Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurLevel();
					// ChangeLevel(pLevel->GetKey());
				}
			}

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::PTR:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(160);

			string Key = "##Pointer";
			Key += ID;

			// vecParam[i].Data는 'Ptr<T>' 또는 'T*' 같은 포인터형 변수를 가리키는 주소임
			// 먼저 저장소 주소 자체가 nullptr인지 체크
			string display = "Empty";

			if (vecParam[i].Data)
			{
				// 저장소의 첫 포인터값을 읽음 (Ptr<T>일 경우 내부 m_Ptr, raw T*일 경우 그 값)
				void* innerPtr = *(void**)(vecParam[i].Data);

				if (innerPtr)
				{
					// 엔진의 모든 게임 오브젝트/에셋이 Entity를 상속하므로 Entity로 캐스트하여 이름 획득
					Entity* pEntity = reinterpret_cast<Entity*>(innerPtr);
					const wstring& wname = pEntity->GetName();
					display = string(wname.begin(), wname.end());
				}
			}

			ImGui::InputText(Key.c_str(), (char*)display.c_str(), display.size() + 1, ImGuiInputTextFlags_ReadOnly);
			AddItemHeight();
		}
			break;
		default:
			break;
		}
		SPACING_UI(2);
		ImGui::Separator();
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
			// LevelMgr의 ChangeLevel은 private 함수
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
