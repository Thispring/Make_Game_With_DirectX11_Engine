#include "pch.h"
#include "EditorUI.h"
#include "imgui/imgui.h"
#include "EditorMgr.h"

EditorUI::EditorUI(const string& _Name)
	: m_Active(true)
	, m_IsModal(false)	// Modal 방식은 필요하면 true로 설정
	, m_Separator(true)
	, m_Parent(nullptr)
	, m_UIName(_Name)
{
}

EditorUI::~EditorUI()
{
}

void EditorUI::Tick()
{
	/**************************************************
	* EditorUI 클래스를 파생하는 자식 클래스의
	* Tick 함수에다, 클래스 목적에 알맞은 기능 구현
	* 
	* 자신이 부모 클래스인지 아닌지와,
	* 그 안에서 자식을 소유하고 있는지의 조건을 체크합니다.
	* 
	* 자식 UI 중 활성화되어 있는 UI만 표시하고
	* 비활성화된 UI에게는 Tick을 보내지 않습니다.
	**************************************************/

	// (26-02-23) Modal 여부에 따라 조건 분기
	if (m_IsModal)
	{
		string StrKey = m_UIName + m_UIKey;
		ImGui::OpenPopup(StrKey.c_str());

		bool Active = m_Active;

		if (ImGui::BeginPopupModal(StrKey.c_str(), &Active, ImGuiWindowFlags_AlwaysAutoResize))
		{
			CheckFocus();

			Tick_UI();

			for (size_t i = 0; i < m_ChildUI.size(); ++i)
			{
				if (m_ChildUI[i]->IsActive())
				{
					m_ChildUI[i]->Tick();
					ImGui::Separator();
				}
			}

			ImGui::EndPopup();
		}
		else
		{
			SetActive(Active);
		}
	}
	else if (m_Parent == nullptr)
	{
		bool Active = m_Active;

		string StrKey = m_UIName + m_UIKey;

		// ImGui Begin에 string 매개변수는, 어떤 창인지 이름을 표시
		ImGui::Begin(StrKey.c_str(), &Active);

		if (m_Active != Active)
		{
			SetActive(Active);
		}

		// Begin과 End사이에서 어떤 윈도우가
		// 선택되었는지 확인
		CheckFocus();

		Tick_UI();	// 자식들이 상속하고 있는 순수 가상 함수

		for (size_t i = 0; i < m_ChildUI.size(); ++i)
		{
			if (m_ChildUI[i]->IsActive())
			{
				m_ChildUI[i]->Tick();
			}
		}

		ImGui::End();
	}
	else
	{
		ImGui::BeginChild(m_UIName.c_str());

		// Begin과 End사이에서 어떤 윈도우가
		// 선택되었는지 확인
		CheckFocus();

		Tick_UI();	// 자식들이 상속하고 있는 순수 가상 함수

		for (size_t i = 0; i < m_ChildUI.size(); ++i)
		{
			if (m_ChildUI[i]->IsActive())
			{
				m_ChildUI[i]->Tick();
				ImGui::Separator();
			}
		}

		ImGui::EndChild();

		// 자식쪽에서 m_Separator가 true일 때만 활성화
		if (m_Separator)
		{
			ImGui::Separator();
		}
	}
	
}

void EditorUI::CheckFocus()
{
	if (ImGui::IsWindowFocused())
		EditorMgr::GetInst()->RegisterFocusedUI(this);
}


//===============
// 자료형 변환 함수
//===============
Vec2::operator ImVec2() const
{
	return ImVec2(x, y);
}

Vec4::operator ImVec4() const
{
	return ImVec4(x, y, z, w);
}
